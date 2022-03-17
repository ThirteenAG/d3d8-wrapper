/**
* Copyright (C) 2020 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#include "d3d8.h"

#pragma comment(lib, "winmm.lib") // needed for timeBeginPeriod()/timeEndPeriod()

Direct3D8EnableMaximizedWindowedModeShimProc m_pDirect3D8EnableMaximizedWindowedModeShim;
ValidatePixelShaderProc m_pValidatePixelShader;
ValidateVertexShaderProc m_pValidateVertexShader;
DebugSetMuteProc m_pDebugSetMute;
Direct3DCreate8Proc m_pDirect3DCreate8;

HWND g_hFocusWindow = NULL;

bool bForceWindowedMode;
bool bDirect3D8DisableMaximizedWindowedModeShim;
bool bUsePrimaryMonitor;
bool bCenterWindow;
bool bBorderlessFullscreen;
bool bAlwaysOnTop;
float fFPSLimit;

class FrameLimiter
{
private:
	static inline double TIME_Frequency = 0.0;
	static inline double TIME_Ticks = 0.0;
	static inline double TIME_Frametime = 0.0;

public:
	enum FPSLimitMode { FPS_NONE, FPS_REALTIME, FPS_ACCURATE };
	static void Init(FPSLimitMode mode)
	{
		LARGE_INTEGER frequency;

		QueryPerformanceFrequency(&frequency);
		static constexpr auto TICKS_PER_FRAME = 1;
		auto TICKS_PER_SECOND = (TICKS_PER_FRAME * fFPSLimit);
		if (mode == FPS_ACCURATE)
		{
			TIME_Frametime = 1000.0 / (double)fFPSLimit;
			TIME_Frequency = (double)frequency.QuadPart / 1000.0; // ticks are milliseconds
		}
		else // FPS_REALTIME
		{
			TIME_Frequency = (double)frequency.QuadPart / (double)TICKS_PER_SECOND; // ticks are 1/n frames (n = fFPSLimit)
		}
		Ticks();
	}
	static DWORD Sync_RT()
	{
		DWORD lastTicks, currentTicks;
		LARGE_INTEGER counter;

		QueryPerformanceCounter(&counter);
		lastTicks = (DWORD)TIME_Ticks;
		TIME_Ticks = (double)counter.QuadPart / TIME_Frequency;
		currentTicks = (DWORD)TIME_Ticks;

		return (currentTicks > lastTicks) ? currentTicks - lastTicks : 0;
	}
	static DWORD Sync_SLP()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		double millis_current = (double)counter.QuadPart / TIME_Frequency;
		double millis_delta = millis_current - TIME_Ticks;
		if (TIME_Frametime <= millis_delta)
		{
			TIME_Ticks = millis_current;
			return 1;
		}
		else if (TIME_Frametime - millis_delta > 2.0) // > 2ms
			Sleep(1); // Sleep for ~1ms
		else
			Sleep(0); // yield thread's time-slice (does not actually sleep)

		return 0;
	}

private:
	static void Ticks()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		TIME_Ticks = (double)counter.QuadPart / TIME_Frequency;
	}
};

FrameLimiter::FPSLimitMode mFPSLimitMode = FrameLimiter::FPSLimitMode::FPS_NONE;

HRESULT m_IDirect3DDevice8::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_REALTIME)
		while (!FrameLimiter::Sync_RT());
	else if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
		while (!FrameLimiter::Sync_SLP());

	return ProxyInterface->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

void ForceWindowed(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	HWND hwnd = pPresentationParameters->hDeviceWindow ? pPresentationParameters->hDeviceWindow : g_hFocusWindow;
	HMONITOR monitor = MonitorFromWindow((!bUsePrimaryMonitor && hwnd) ? hwnd : GetDesktopWindow(), MONITOR_DEFAULTTONEAREST);
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(monitor, &info);
	int DesktopResX = info.rcMonitor.right - info.rcMonitor.left;
	int DesktopResY = info.rcMonitor.bottom - info.rcMonitor.top;

	int left = (int)info.rcMonitor.left;
	int top = (int)info.rcMonitor.top;

	if (!bBorderlessFullscreen)
	{
		left += (int)(((float)DesktopResX / 2.0f) - ((float)pPresentationParameters->BackBufferWidth / 2.0f));
		top += (int)(((float)DesktopResY / 2.0f) - ((float)pPresentationParameters->BackBufferHeight / 2.0f));
	}

	pPresentationParameters->Windowed = 1;

	// These must be set to default (0) on windowed mode as per spec to prevent app freezing
	pPresentationParameters->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pPresentationParameters->FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if (hwnd != NULL)
	{
		UINT uFlags = SWP_SHOWWINDOW;
		if (bBorderlessFullscreen)
		{
			LONG lOldStyle = GetWindowLong(hwnd, GWL_STYLE);
			LONG lOldExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
			LONG lNewStyle = lOldStyle & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_DLGFRAME);
			lNewStyle |= (lOldStyle & WS_CHILD) ? 0 : WS_POPUP;
			LONG lNewExStyle = lOldExStyle & ~(WS_EX_CONTEXTHELP | WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW);
			lNewExStyle |= WS_EX_APPWINDOW;

			if (lNewStyle != lOldStyle)
			{
				SetWindowLong(hwnd, GWL_STYLE, lNewStyle);
				uFlags |= SWP_FRAMECHANGED;
			}
			if (lNewExStyle != lOldExStyle)
			{
				SetWindowLong(hwnd, GWL_EXSTYLE, lNewExStyle);
				uFlags |= SWP_FRAMECHANGED;
			}
			SetWindowPos(hwnd, bAlwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, left, top, DesktopResX, DesktopResY, uFlags);
		}
		else
		{
			if (!bCenterWindow)
				uFlags |= SWP_NOMOVE;

			SetWindowPos(hwnd, bAlwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST, left, top, pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, uFlags);
		}
	}
}

HRESULT m_IDirect3D8::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface)
{
	if (bForceWindowedMode)
	{
		g_hFocusWindow = hFocusWindow;
		ForceWindowed(pPresentationParameters);
	}

	HRESULT hr = ProxyInterface->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);

	if (SUCCEEDED(hr) && ppReturnedDeviceInterface)
	{
		*ppReturnedDeviceInterface = new m_IDirect3DDevice8(*ppReturnedDeviceInterface, this);
	}
	return hr;
}

HRESULT m_IDirect3DDevice8::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (bForceWindowedMode)
		ForceWindowed(pPresentationParameters);

	return ProxyInterface->Reset(pPresentationParameters);
}

bool WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	static HMODULE d3d8dll = nullptr;

	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			// Load dll
			char path[MAX_PATH];
			GetSystemDirectoryA(path, MAX_PATH);
			strcat_s(path, "\\d3d8.dll");
			d3d8dll = LoadLibraryA(path);

			// Get function addresses
			m_pDirect3D8EnableMaximizedWindowedModeShim = (Direct3D8EnableMaximizedWindowedModeShimProc)GetProcAddress(d3d8dll, "Direct3D8EnableMaximizedWindowedModeShim");
			m_pValidatePixelShader = (ValidatePixelShaderProc)GetProcAddress(d3d8dll, "ValidatePixelShader");
			m_pValidateVertexShader = (ValidateVertexShaderProc)GetProcAddress(d3d8dll, "ValidateVertexShader");
			m_pDebugSetMute = (DebugSetMuteProc)GetProcAddress(d3d8dll, "DebugSetMute");
			m_pDirect3DCreate8 = (Direct3DCreate8Proc)GetProcAddress(d3d8dll, "Direct3DCreate8");

			//ini
			HMODULE hm = NULL;
			GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&Direct3DCreate8, &hm);
			GetModuleFileNameA(hm, path, sizeof(path));
			*strrchr(path, '\\') = '\0';
			strcat_s(path, "\\d3d8.ini");

			bForceWindowedMode = GetPrivateProfileInt("MAIN", "ForceWindowedMode", 0, path) != 0;
			bDirect3D8DisableMaximizedWindowedModeShim = GetPrivateProfileInt("MAIN", "Direct3D8DisableMaximizedWindowedModeShim", 0, path) != 0;
			fFPSLimit = static_cast<float>(GetPrivateProfileInt("MAIN", "FPSLimit", 0, path));
			bUsePrimaryMonitor = GetPrivateProfileInt("FORCEWINDOWED", "UsePrimaryMonitor", 0, path) != 0;
			bCenterWindow = GetPrivateProfileInt("FORCEWINDOWED", "CenterWindow", 1, path) != 0;
			bBorderlessFullscreen = GetPrivateProfileInt("FORCEWINDOWED", "BorderlessFullscreen", 0, path) != 0;
			bAlwaysOnTop = GetPrivateProfileInt("FORCEWINDOWED", "AlwaysOnTop", 0, path) != 0;

			if (fFPSLimit > 0.0f)
			{
				FrameLimiter::FPSLimitMode mode = (GetPrivateProfileInt("MAIN", "FPSLimitMode", 1, path) == 2) ? FrameLimiter::FPSLimitMode::FPS_ACCURATE : FrameLimiter::FPSLimitMode::FPS_REALTIME;
				if (mode == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
					timeBeginPeriod(1);

				FrameLimiter::Init(mode);
				mFPSLimitMode = mode;
			}
			else
				mFPSLimitMode = FrameLimiter::FPSLimitMode::FPS_NONE;

			if (bDirect3D8DisableMaximizedWindowedModeShim)
			{
				auto addr = (uintptr_t)GetProcAddress(d3d8dll, "Direct3D8EnableMaximizedWindowedModeShim");
				if (addr)
				{
					DWORD Protect;
					VirtualProtect((LPVOID)(addr + 6), 4, PAGE_EXECUTE_READWRITE, &Protect);
					*(unsigned*)(addr + 6) = 0;
					*(unsigned*)(*(unsigned*)(addr + 2)) = 0;
					VirtualProtect((LPVOID)(addr + 6), 4, Protect, &Protect);
					bForceWindowedMode = false;
				}
			}
		}
		break;
		case DLL_PROCESS_DETACH:
		{
			if (mFPSLimitMode == FrameLimiter::FPSLimitMode::FPS_ACCURATE)
				timeEndPeriod(1);

			FreeLibrary(d3d8dll);
		}
		break;
	}

	return true;
}

int WINAPI Direct3D8EnableMaximizedWindowedModeShim(BOOL mEnable)
{
	if (!m_pDirect3D8EnableMaximizedWindowedModeShim)
	{
		return E_FAIL;
	}

	return m_pDirect3D8EnableMaximizedWindowedModeShim(mEnable);
}

HRESULT WINAPI ValidatePixelShader(DWORD* pixelshader, DWORD* reserved1, BOOL flag, DWORD* toto)
{
	if (!m_pValidatePixelShader)
	{
		return E_FAIL;
	}

	return m_pValidatePixelShader(pixelshader, reserved1, flag, toto);
}

HRESULT WINAPI ValidateVertexShader(DWORD* vertexshader, DWORD* reserved1, DWORD* reserved2, BOOL flag, DWORD* toto)
{
	if (!m_pValidateVertexShader)
	{
		return E_FAIL;
	}

	return m_pValidateVertexShader(vertexshader, reserved1, reserved2, flag, toto);
}

void WINAPI DebugSetMute()
{
	if (!m_pDebugSetMute)
	{
		return;
	}

	return m_pDebugSetMute();
}

IDirect3D8 *WINAPI Direct3DCreate8(UINT SDKVersion)
{
	if (!m_pDirect3DCreate8)
	{
		return nullptr;
	}

	IDirect3D8 *pD3D8 = m_pDirect3DCreate8(SDKVersion);

	if (pD3D8)
	{
		return new m_IDirect3D8(pD3D8);
	}

	return nullptr;
}
