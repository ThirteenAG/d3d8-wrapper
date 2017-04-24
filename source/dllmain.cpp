#include <windows.h>
#include <d3d8.h>
#include <math.h>
#include "Direct3D8Wrapper.h"
#include "Direct3DDevice8Wrapper.h"

//Sources used from:
//https://bitbucket.org/andrewcooper/windower_open
//http://www.jiri-dvorak.cz/scellpt/index.html

bool bForceWindowedMode;
bool bSetVertexShaderConstantHook;
bool bDirect3D8DisableMaximizedWindowedModeShim;
bool bFPSLimit;
float fFPSLimit;

struct d3d8_dll
{
  HMODULE dll;
  FARPROC DebugSetMute_d3d8;
  //FARPROC Direct3D8EnableMaximizedWindowedModeShim;
  FARPROC Direct3DCreate8;
  FARPROC ValidatePixelShader;
  FARPROC ValidateVertexShader;
} d3d8;

__declspec(naked) void _DebugSetMute_d3d8() { _asm { jmp [d3d8.DebugSetMute_d3d8] } }
//__declspec(naked) void _Direct3D8EnableMaximizedWindowedModeShim() { _asm { jmp [d3d8.Direct3D8EnableMaximizedWindowedModeShim] } }
__declspec(naked) void _Direct3DCreate8() { _asm { jmp [d3d8.Direct3DCreate8] } }
__declspec(naked) void _ValidatePixelShader() { _asm { jmp [d3d8.ValidatePixelShader] } }
__declspec(naked) void _ValidateVertexShader() { _asm { jmp [d3d8.ValidateVertexShader] } }
IDirect3D8*	(WINAPI *OriginalDirect3DCreate8) (UINT SDKVersion);

IDirect3D8* WINAPI Direct3DCreate8Callback(UINT SDKVersion)
{
    IDirect3D8* Direct3D = OriginalDirect3DCreate8(SDKVersion);
    IDirect3D8* WrappedDirect3D = new Direct3D8Wrapper(Direct3D);
    return WrappedDirect3D;
}

HRESULT Direct3DDevice8Wrapper::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion)
{
    if (bFPSLimit)
    {
        static LARGE_INTEGER Frequency;
        static LARGE_INTEGER currCounter;
        static LARGE_INTEGER startCounter;
        static DWORD nextTicks;
        static float curFPS;

        static DWORD updateInterval = 1000;
        static DWORD startTime = 0;
        static DWORD nfps = 0;

        nfps++;

        auto dt = GetTickCount() - startTime;
        curFPS = (float)(nfps*1000.0 / (float)dt);

        if (dt > updateInterval) {
            nfps = 0;
            startTime = GetTickCount() + 1;
        }

        QueryPerformanceCounter(&currCounter);
        while (currCounter.QuadPart <= (startCounter.QuadPart + nextTicks))
        {
            if (curFPS > fFPSLimit)
            {
                Sleep(1);
            }
            else
                break;

            QueryPerformanceCounter(&currCounter);
        }
        auto hr = Direct3DDevice8->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
        QueryPerformanceFrequency(&Frequency);
        nextTicks = (DWORD)((float)Frequency.QuadPart / fFPSLimit);
        currCounter.QuadPart = 0;
        startCounter.QuadPart = 0;
        QueryPerformanceCounter(&startCounter);

        return hr;
    }
    else
        return Direct3DDevice8->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

void ForceWindowed(D3DPRESENT_PARAMETERS *pPresentationParameters)
{
    HMONITOR monitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    int DesktopResX = info.rcMonitor.right - info.rcMonitor.left;
    int DesktopResY = info.rcMonitor.bottom - info.rcMonitor.top;

    int left = (int)(((float)DesktopResX / 2.0f) - ((float)pPresentationParameters->BackBufferWidth / 2.0f));
    int top = (int)(((float)DesktopResY / 2.0f) - ((float)pPresentationParameters->BackBufferHeight / 2.0f));

    pPresentationParameters->Windowed = true;

    SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, left, top, pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, SWP_SHOWWINDOW);
}

HRESULT Direct3D8Wrapper::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface) {
    IDirect3DDevice8* Direct3DDevice8;

    if (bForceWindowedMode)
        ForceWindowed(pPresentationParameters);

    HRESULT Result = Direct3D8->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &Direct3DDevice8);
    *ppReturnedDeviceInterface = new Direct3DDevice8Wrapper(&Direct3DDevice8, pPresentationParameters);
    return Result;
}

HRESULT Direct3DDevice8Wrapper::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) {
    OutputDebugString("Device reset called.");

    if (bForceWindowedMode)
        ForceWindowed(pPresentationParameters);

    return Direct3DDevice8->Reset(PresentationParameters);
}

HRESULT Direct3DDevice8Wrapper::SetVertexShaderConstant(DWORD Register, CONST void *pConstantData, DWORD ConstantCount) {

    const float *const ConstValues = static_cast<const float *>(pConstantData);

    // It must look like matrix. Note that different games might upload
    // constants as a bigger block in which case this will not process
    // the matrix and game-specific knowledge (or analysis of the shader)
    // needs to be applied.

    if (ConstantCount != 4 || !bSetVertexShaderConstantHook) {
        return Direct3DDevice8->SetVertexShaderConstant(Register, pConstantData, ConstantCount);
    }

    // For use with SC assume 24bit shadow textures.

    const unsigned texture_bit_depth = (1 << 24) - 1;

    // In DirectX8 interfaces the Z component was supposed to be provided
    // as prescaled by (2^bitdepth - 1). Check if the corresponding part
    // of matrix looks prescaled.

    if (fabs(ConstValues[11]) < 300000) {
        return Direct3DDevice8->SetVertexShaderConstant(Register, pConstantData, ConstantCount);
    }

    // The DirectX9 interfaces expects it in its native <0;1> range. It
    // seems that starting from some driver version the new behavior
    // applies also to the old version of the interface. Revert the scale.

    float compensated_matrix[4 * 4];
    memcpy(compensated_matrix, pConstantData, sizeof(compensated_matrix));

    const float compensation_factor = 1.0f / texture_bit_depth;
    for (unsigned i = 0; i < 4; ++i) {
        compensated_matrix[8 + i] *= compensation_factor;
    }

    return Direct3DDevice8->SetVertexShaderConstant(Register, compensated_matrix, ConstantCount);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  char path[MAX_PATH];
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    {
        CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 9), "\\d3d8.dll", 10);
        d3d8.dll = LoadLibrary(path);
        if (d3d8.dll == false)
        {
            ExitProcess(0);
        }

        d3d8.DebugSetMute_d3d8 = GetProcAddress(d3d8.dll, "DebugSetMute");
        //d3d8.Direct3D8EnableMaximizedWindowedModeShim = GetProcAddress(d3d8.dll, "Direct3D8EnableMaximizedWindowedModeShim");
        d3d8.Direct3DCreate8 = GetProcAddress(d3d8.dll, "Direct3DCreate8");
        d3d8.ValidatePixelShader = GetProcAddress(d3d8.dll, "ValidatePixelShader");
        d3d8.ValidateVertexShader = GetProcAddress(d3d8.dll, "ValidateVertexShader");

        //wrapping Direct3DCreate8
        OriginalDirect3DCreate8 = (IDirect3D8 *(__stdcall *)(UINT))GetProcAddress(d3d8.dll, "Direct3DCreate8");
        d3d8.Direct3DCreate8 = (FARPROC)Direct3DCreate8Callback;

        //ini
        char path[MAX_PATH];
        HMODULE hm = NULL;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&Direct3DCreate8Callback, &hm);
        GetModuleFileNameA(hm, path, sizeof(path));
        *strrchr(path, '\\') = '\0';
        strcat_s(path, "\\d3d8.ini");

        bForceWindowedMode = GetPrivateProfileInt("MAIN", "ForceWindowedMode", 0, path) != 0;
        bSetVertexShaderConstantHook = GetPrivateProfileInt("MAIN", "SetVertexShaderConstantHook", 0, path) != 0;
        bDirect3D8DisableMaximizedWindowedModeShim = GetPrivateProfileInt("MAIN", "Direct3D8DisableMaximizedWindowedModeShim", 0, path) != 0;
        fFPSLimit = static_cast<float>(GetPrivateProfileInt("MAIN", "FPSLimit", 0, path));
        if (fFPSLimit)
            bFPSLimit = true;

        if (bDirect3D8DisableMaximizedWindowedModeShim)
        {
            auto addr = (uintptr_t)GetProcAddress(d3d8.dll, "Direct3D8EnableMaximizedWindowedModeShim");
            if (addr)
            {
                DWORD Protect;
                VirtualProtect((LPVOID)(addr + 6), 4, PAGE_EXECUTE_READWRITE, &Protect);
                *(unsigned*)(addr + 6) = 0;
                *(unsigned*)(*(unsigned*)(addr + 2)) = 0;
                VirtualProtect((LPVOID)(addr + 6), 4, Protect, &Protect);
                bForceWindowedMode = 0;
            }
        }
    }
    break;

    case DLL_PROCESS_DETACH:
      FreeLibrary(d3d8.dll);
    break;
  }
  return TRUE;
}