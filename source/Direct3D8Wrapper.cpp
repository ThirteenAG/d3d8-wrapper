#include <windows.h>
#include <d3d8.h>
#include "Direct3D8Wrapper.h"
#include "Direct3DDevice8Wrapper.h"

//class
Direct3D8Wrapper::Direct3D8Wrapper(LPDIRECT3D8 pDirect3D) {
    Direct3D8 = pDirect3D;
}

Direct3D8Wrapper::~Direct3D8Wrapper() {
}

ULONG Direct3D8Wrapper::AddRef() {
    return Direct3D8->AddRef();
}

HRESULT Direct3D8Wrapper::QueryInterface(REFIID riid, LPVOID *ppvObj) {
    *ppvObj = NULL;
    HRESULT hr = Direct3D8->QueryInterface(riid, ppvObj);
    if (hr == D3D_OK)
        *ppvObj = this;
    return hr;
}

ULONG Direct3D8Wrapper::Release() {
    ULONG count = Direct3D8->Release();
    if (count == 0) {
        delete(this);
    }
    return(count);
}

//HRESULT Direct3D8Wrapper::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface) {
//	IDirect3DDevice8* Direct3DDevice8;
//	HRESULT Result = Direct3D8->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &Direct3DDevice8);
//	*ppReturnedDeviceInterface = new Direct3DDevice8Wrapper(&Direct3DDevice8, pPresentationParameters);
//	return Result;
//}

HRESULT Direct3D8Wrapper::EnumAdapterModes(UINT Adapter, UINT Mode, D3DDISPLAYMODE *pMode) {
    return Direct3D8->EnumAdapterModes(Adapter, Mode, pMode);
}

UINT Direct3D8Wrapper::GetAdapterCount() {
    return Direct3D8->GetAdapterCount();
}

HRESULT Direct3D8Wrapper::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE *pMode) {
    return Direct3D8->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT Direct3D8Wrapper::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 *pIdentifier) {
    return Direct3D8->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT Direct3D8Wrapper::GetAdapterModeCount(UINT Adapter) {
    return Direct3D8->GetAdapterModeCount(Adapter);
}

HMONITOR Direct3D8Wrapper::GetAdapterMonitor(UINT Adapter) {
    return Direct3D8->GetAdapterMonitor(Adapter);
}

HRESULT Direct3D8Wrapper::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 *pCaps) {
    return Direct3D8->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HRESULT Direct3D8Wrapper::RegisterSoftwareDevice(void *pInitializeFunction) {
    return Direct3D8->RegisterSoftwareDevice(pInitializeFunction);
}

HRESULT Direct3D8Wrapper::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) {
    return Direct3D8->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT Direct3D8Wrapper::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) {
    return Direct3D8->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT Direct3D8Wrapper::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType) {
    return Direct3D8->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType);
}

HRESULT Direct3D8Wrapper::CheckDeviceType(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed) {
    return Direct3D8->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}
