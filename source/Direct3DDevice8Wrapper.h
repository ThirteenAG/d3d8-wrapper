#include <windows.h>
#include <d3d8.h>

class Direct3DDevice8Wrapper : public IDirect3DDevice8 {

public:

	Direct3DDevice8Wrapper(IDirect3DDevice8* *pDirect3DDevice8, D3DPRESENT_PARAMETERS* pPresentationParameters);
	virtual ~Direct3DDevice8Wrapper();

	//IUnknown
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObj);
	STDMETHOD_(ULONG, Release)(THIS);
	//Cursors
	STDMETHOD_(void, SetCursorPosition)(int XScreenSpace, int YScreenSpace, DWORD Flags);
	STDMETHOD(SetCursorProperties)(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap);
	STDMETHOD_(BOOL, ShowCursor)(BOOL bShow);
	//Creation
	STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain8 **pSwapChain);
	STDMETHOD(CreateCubeTexture)(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 **ppCubeTexture);
	STDMETHOD(CreateDepthStencilSurface)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 **ppSurface);
	STDMETHOD(CreateImageSurface)(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface);
	STDMETHOD(CreateIndexBuffer)(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer);
	STDMETHOD(CreateRenderTarget)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8 **ppSurface);
	STDMETHOD(CreateTexture)(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 **ppTexture);
	STDMETHOD(CreateVertexBuffer)(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer);
	STDMETHOD(CreateVolumeTexture)(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 **ppVolumeTexture);
	//Device States
	STDMETHOD(ApplyStateBlock)(DWORD Token);
	STDMETHOD(BeginStateBlock)(THIS);
	STDMETHOD(CaptureStateBlock)(DWORD Token);
	STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type, DWORD *pToken);
	STDMETHOD(DeleteStateBlock)(DWORD Token);
	STDMETHOD(EndStateBlock)(DWORD *pToken);
	STDMETHOD(GetClipStatus)(D3DCLIPSTATUS8 *pClipStatus);
	STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE *pMode);
	STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State, DWORD *pValue);
	STDMETHOD(GetRenderTarget)(IDirect3DSurface8 **ppRenderTarget);
	STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix);
	STDMETHOD(SetClipStatus)(CONST D3DCLIPSTATUS8 *pClipStatus);
	STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State, DWORD Value);
	STDMETHOD(SetRenderTarget)(IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8 *pNewZStencil);
	STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix);
	//Gamma Ramps
	STDMETHOD_(void, GetGammaRamp)(D3DGAMMARAMP *pRamp);
	STDMETHOD_(void, SetGammaRamp)(DWORD Flags, CONST D3DGAMMARAMP *pRamp);
	//High-Order Primitives
	STDMETHOD(DeletePatch)(UINT Handle);
	STDMETHOD(DrawRectPatch)(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo);
	STDMETHOD(DrawTriPatch)(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo);
	//Index Data
	STDMETHOD(GetIndices)(IDirect3DIndexBuffer8 **ppIndexData, UINT *pBaseVertexIndex);
	STDMETHOD(SetIndices)(IDirect3DIndexBuffer8 *pIndexData, UINT BaseVertexIndex);
	//Information
	STDMETHOD_(UINT, GetAvailableTextureMem)(THIS);
	STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS *pParameters);
	STDMETHOD(GetDeviceCaps)(D3DCAPS8 *pCaps);
	STDMETHOD(GetDirect3D)(IDirect3D8 **ppD3D8);
	STDMETHOD(GetInfo)(DWORD DevInfoID, void *pDevInfoStruct, DWORD DevInfoStructSize);
	STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS *pRasterStatus);
	//Lighting and Materials
	STDMETHOD(GetLight)(DWORD Index, D3DLIGHT8 *pLight);
	STDMETHOD(GetLightEnable)(DWORD Index, BOOL *pEnable);
	STDMETHOD(GetMaterial)(D3DMATERIAL8 *pMaterial);
	STDMETHOD(LightEnable)(DWORD Index, BOOL Enable);
	STDMETHOD(SetLight)(DWORD Index, CONST D3DLIGHT8 *pLight);
	STDMETHOD(SetMaterial)(CONST D3DMATERIAL8 *pMaterial);
	//Miscellaneous
	STDMETHOD(CopyRects)(IDirect3DSurface8 *pSourceSurface, CONST RECT *pSourceRectsArray, UINT cRects, IDirect3DSurface8 *pDestinationSurface, CONST POINT *pDestPointsArray);
	STDMETHOD(GetFrontBuffer)(IDirect3DSurface8 *pDestSurface);
	STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE, CONST D3DMATRIX*);
	STDMETHOD(ProcessVertices)(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8 *pDestBuffer, DWORD Flags);
	STDMETHOD(ResourceManagerDiscardBytes)(DWORD Bytes);
	STDMETHOD(TestCooperativeLevel)(THIS);
	//Palettes
	STDMETHOD(GetCurrentTexturePalette)(UINT *PaletteNumber);
	STDMETHOD(GetPaletteEntries)(UINT PaletteNumber, PALETTEENTRY *pEntries);
	STDMETHOD(SetCurrentTexturePalette)(UINT PaletteNumber);
	STDMETHOD(SetPaletteEntries)(UINT PaletteNumber, CONST PALETTEENTRY *pEntries);
	//Pixel Shaders
	STDMETHOD(CreatePixelShader)(CONST DWORD *pFunction, DWORD *pHandle);
	STDMETHOD(DeletePixelShader)(DWORD Handle);
	STDMETHOD(GetPixelShader)(DWORD *pHandle);
	STDMETHOD(GetPixelShaderConstant)(DWORD Register, void *pConstantData, DWORD ConstantCount);
	STDMETHOD(GetPixelShaderFunction)(DWORD Handle, void *pData, DWORD *pSizeOfData);
	STDMETHOD(SetPixelShader)(DWORD Handle);
	STDMETHOD(SetPixelShaderConstant)(DWORD Register, CONST void *pConstantData, DWORD ConstantCount);
	//Presentation
	STDMETHOD(Present)(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion);
	STDMETHOD(Reset)(D3DPRESENT_PARAMETERS *pPresentationParameters);
	//Rendering
	STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount);
	STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
	STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride);
	//Scenes
	STDMETHOD(BeginScene)(THIS);
	STDMETHOD(EndScene)(THIS);
	//Stream Data
	STDMETHOD(GetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, UINT *pStride);
	STDMETHOD(SetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer8 *pStreamData, UINT Stride);
	//Surfaces
	STDMETHOD(GetBackBuffer)(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer);
	STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface8 **ppZStencilSurface);
	//Textures
	STDMETHOD(GetTexture)(DWORD Stage, IDirect3DBaseTexture8 **ppTexture);
	STDMETHOD(GetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue);
	STDMETHOD(SetTexture)(DWORD Stage, IDirect3DBaseTexture8 *pTexture);
	STDMETHOD(SetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
	STDMETHOD(UpdateTexture)(IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture);
	STDMETHOD(ValidateDevice)(DWORD *pNumPasses);
	//User-Defined Clip Planes
	STDMETHOD(GetClipPlane)(DWORD Index, float *pPlane);
	STDMETHOD(SetClipPlane)(DWORD Index, CONST float *pPlane);
	//Viewports
	STDMETHOD(Clear)(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
	STDMETHOD(GetViewport)(D3DVIEWPORT8 *pViewport);
	STDMETHOD(SetViewport)(CONST D3DVIEWPORT8 *pViewport);
	//Vertex Shaders
	STDMETHOD(CreateVertexShader)(CONST DWORD *pDeclaration, CONST DWORD *pFunction, DWORD *pHandle, DWORD Usage);
	STDMETHOD(DeleteVertexShader)(DWORD Handle);
	STDMETHOD(GetVertexShader)(DWORD *pHandle);
	STDMETHOD(GetVertexShaderConstant)(DWORD Register, void *pConstantData, DWORD ConstantCount);
	STDMETHOD(GetVertexShaderDeclaration)(DWORD Handle, void *pData, DWORD *pSizeOfData);
	STDMETHOD(GetVertexShaderFunction)(DWORD Handle, void *pData, DWORD *pSizeOfData);
	STDMETHOD(SetVertexShader)(DWORD Handle);
	STDMETHOD(SetVertexShaderConstant)(DWORD Register, CONST void *pConstantData, DWORD ConstantCount);

	IDirect3DDevice8* Direct3DDevice8;
	D3DPRESENT_PARAMETERS* PresentationParameters;
};
