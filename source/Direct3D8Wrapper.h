#include <windows.h>
#include <d3d8.h>

class Direct3D8Wrapper : public IDirect3D8 {

public:
    Direct3D8Wrapper(LPDIRECT3D8 pDirect3D);
    virtual ~Direct3D8Wrapper();

    // IUnknown
    STDMETHOD_(ULONG, AddRef)(THIS);
    STDMETHOD(QueryInterface)(REFIID riid,  void **ppvObj);
    STDMETHOD_(ULONG, Release)(THIS);
    // Creation
    STDMETHOD(CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface);
    // Enumeration
    STDMETHOD(EnumAdapterModes)(UINT Adapter, UINT Mode, D3DDISPLAYMODE *pMode);
    // Information
    STDMETHOD_(UINT, GetAdapterCount)(THIS);
    STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, D3DDISPLAYMODE *pMode);
    STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8 *pIdentifier);
    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter);
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter);
    STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8 *pCaps);
    // Registration
    STDMETHOD(RegisterSoftwareDevice)(void *pInitializeFunction);
    // Verification
    STDMETHOD(CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    STDMETHOD(CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType);
    STDMETHOD(CheckDeviceType)(UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed);

    //variables
    IDirect3D8* Direct3D8;
};
