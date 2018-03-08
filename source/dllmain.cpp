#include <windows.h>
#include <d3d8.h>
#include <math.h>
#include "Direct3D8Wrapper.h"
#include "Direct3DDevice8Wrapper.h"

//Sources used from:
//https://bitbucket.org/andrewcooper/windower_open

bool bForceWindowedMode;
bool bDirect3D8DisableMaximizedWindowedModeShim;
bool bFPSLimit;
float fFPSLimit;

struct d3d8_dll
{
    HMODULE dll;
    FARPROC DebugSetMute;
    FARPROC Direct3D8EnableMaximizedWindowedModeShim;
    FARPROC Direct3DCreate8;
    FARPROC ValidatePixelShader;
    FARPROC ValidateVertexShader;
} d3d8;

__declspec(naked) void _DebugSetMute() { _asm { jmp[d3d8.DebugSetMute] } }
__declspec(naked) void _Direct3D8EnableMaximizedWindowedModeShim() { _asm { jmp[d3d8.Direct3D8EnableMaximizedWindowedModeShim] } }
__declspec(naked) void _Direct3DCreate8() { _asm { jmp[d3d8.Direct3DCreate8] } }
__declspec(naked) void _ValidatePixelShader() { _asm { jmp[d3d8.ValidatePixelShader] } }
__declspec(naked) void _ValidateVertexShader() { _asm { jmp[d3d8.ValidateVertexShader] } }
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

        d3d8.DebugSetMute = GetProcAddress(d3d8.dll, "DebugSetMute");
        d3d8.Direct3D8EnableMaximizedWindowedModeShim = GetProcAddress(d3d8.dll, "Direct3D8EnableMaximizedWindowedModeShim");
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