#include <windows.h>
#include <d3d8.h>
#include "Direct3DDevice8Wrapper.h"

Direct3DDevice8Wrapper::Direct3DDevice8Wrapper(IDirect3DDevice8* *pDirect3DDevice8, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    Direct3DDevice8 = *pDirect3DDevice8;

    PresentationParameters = new D3DPRESENT_PARAMETERS();
    *PresentationParameters = *pPresentationParameters;
}

Direct3DDevice8Wrapper::~Direct3DDevice8Wrapper() {
    delete PresentationParameters;
}

ULONG Direct3DDevice8Wrapper::AddRef() {
    return Direct3DDevice8->AddRef();
}

HRESULT Direct3DDevice8Wrapper::QueryInterface(REFIID riid, LPVOID *ppvObj) {
    *ppvObj = NULL;
    HRESULT hr = Direct3DDevice8->QueryInterface(riid, ppvObj);
    if (hr == D3D_OK)
        *ppvObj = this;
    return hr;
}

ULONG Direct3DDevice8Wrapper::Release() {
    ULONG count = Direct3DDevice8->Release();
    if (count == 0) {
        delete(this);
    }
    return (count);
}

void Direct3DDevice8Wrapper::SetCursorPosition(int XScreenSpace, int YScreenSpace, DWORD Flags) {
    Direct3DDevice8->SetCursorPosition(XScreenSpace, YScreenSpace, Flags);
}

HRESULT Direct3DDevice8Wrapper::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8 *pCursorBitmap) {
    return Direct3DDevice8->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

BOOL Direct3DDevice8Wrapper::ShowCursor(BOOL bShow) {
    return Direct3DDevice8->ShowCursor(bShow);
}

HRESULT Direct3DDevice8Wrapper::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain8 **ppSwapChain) {
    return Direct3DDevice8->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT Direct3DDevice8Wrapper::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 **ppCubeTexture) {
    return Direct3DDevice8->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture);
}

HRESULT Direct3DDevice8Wrapper::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 **ppSurface) {
    return Direct3DDevice8->CreateDepthStencilSurface(Width, Height, Format, MultiSample, ppSurface);
}

HRESULT Direct3DDevice8Wrapper::CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface) {
    return Direct3DDevice8->CreateImageSurface(Width, Height, Format, ppSurface);
}

HRESULT Direct3DDevice8Wrapper::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer) {
    return Direct3DDevice8->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer);
}

HRESULT Direct3DDevice8Wrapper::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8 **ppSurface) {
    return Direct3DDevice8->CreateRenderTarget(Width, Height, Format, MultiSample, Lockable, ppSurface);
}

HRESULT Direct3DDevice8Wrapper::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 **ppTexture) {
    return Direct3DDevice8->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture);
}

HRESULT Direct3DDevice8Wrapper::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer) {
    return Direct3DDevice8->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer);
}

HRESULT Direct3DDevice8Wrapper::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 **ppVolumeTexture) {
    return Direct3DDevice8->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture);
}

HRESULT Direct3DDevice8Wrapper::ApplyStateBlock(DWORD Token) {
    return Direct3DDevice8->ApplyStateBlock(Token);
}

HRESULT Direct3DDevice8Wrapper::BeginStateBlock() {
    return Direct3DDevice8->BeginStateBlock();
}

HRESULT Direct3DDevice8Wrapper::CaptureStateBlock(DWORD Token) {
    return Direct3DDevice8->CaptureStateBlock(Token);
}

HRESULT Direct3DDevice8Wrapper::CreateStateBlock(D3DSTATEBLOCKTYPE Type, DWORD *pToken) {
    return Direct3DDevice8->CreateStateBlock(Type, pToken);
}

HRESULT Direct3DDevice8Wrapper::DeleteStateBlock(DWORD Token) {
    return Direct3DDevice8->DeleteStateBlock(Token);
}

HRESULT Direct3DDevice8Wrapper::EndStateBlock(DWORD *pToken) {
    return Direct3DDevice8->EndStateBlock(pToken);
}

HRESULT Direct3DDevice8Wrapper::GetClipStatus(D3DCLIPSTATUS8 *pClipStatus) {
    return Direct3DDevice8->GetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice8Wrapper::GetDisplayMode(D3DDISPLAYMODE *pMode) {
    return Direct3DDevice8->GetDisplayMode(pMode);
}

HRESULT Direct3DDevice8Wrapper::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) {
    return Direct3DDevice8->GetRenderState(State, pValue);
}

HRESULT Direct3DDevice8Wrapper::GetRenderTarget(IDirect3DSurface8 **ppRenderTarget) {
    return Direct3DDevice8->GetRenderTarget(ppRenderTarget);
}

HRESULT Direct3DDevice8Wrapper::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix) {
    return Direct3DDevice8->GetTransform(State, pMatrix);
}

HRESULT Direct3DDevice8Wrapper::SetClipStatus(CONST D3DCLIPSTATUS8 *pClipStatus) {
    return Direct3DDevice8->SetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice8Wrapper::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) {
    return Direct3DDevice8->SetRenderState(State, Value);
}

HRESULT Direct3DDevice8Wrapper::SetRenderTarget(IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8 *pNewZStencil) {
    return Direct3DDevice8->SetRenderTarget(pRenderTarget, pNewZStencil);
}

HRESULT Direct3DDevice8Wrapper::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) {
    return Direct3DDevice8->SetTransform(State, pMatrix);
}

void Direct3DDevice8Wrapper::GetGammaRamp(D3DGAMMARAMP *pRamp) {
    Direct3DDevice8->GetGammaRamp(pRamp);
}

void Direct3DDevice8Wrapper::SetGammaRamp(DWORD Flags, CONST D3DGAMMARAMP *pRamp) {
    Direct3DDevice8->SetGammaRamp(Flags, pRamp);
}

HRESULT Direct3DDevice8Wrapper::DeletePatch(UINT Handle) {
    return Direct3DDevice8->DeletePatch(Handle);
}

HRESULT Direct3DDevice8Wrapper::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) {
    return Direct3DDevice8->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT Direct3DDevice8Wrapper::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo) {
    return Direct3DDevice8->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT Direct3DDevice8Wrapper::GetIndices(IDirect3DIndexBuffer8 **ppIndexData, UINT *pBaseVertexIndex) {
    return Direct3DDevice8->GetIndices(ppIndexData, pBaseVertexIndex);
}

HRESULT Direct3DDevice8Wrapper::SetIndices(IDirect3DIndexBuffer8 *pIndexData, UINT BaseVertexIndex) {
    return Direct3DDevice8->SetIndices(pIndexData, BaseVertexIndex);
}

UINT Direct3DDevice8Wrapper::GetAvailableTextureMem() {
    return Direct3DDevice8->GetAvailableTextureMem();
}

HRESULT Direct3DDevice8Wrapper::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) {
    return Direct3DDevice8->GetCreationParameters(pParameters);
}

HRESULT Direct3DDevice8Wrapper::GetDeviceCaps(D3DCAPS8 *pCaps) {
    return Direct3DDevice8->GetDeviceCaps(pCaps);
}

HRESULT Direct3DDevice8Wrapper::GetDirect3D(IDirect3D8 **ppD3D8) {
    return Direct3DDevice8->GetDirect3D(ppD3D8);
}

HRESULT Direct3DDevice8Wrapper::GetInfo(DWORD DevInfoID, VOID *pDevInfoStruct, DWORD DevInfoStructSize) {
    return Direct3DDevice8->GetInfo(DevInfoID, pDevInfoStruct, DevInfoStructSize);
}

HRESULT Direct3DDevice8Wrapper::GetRasterStatus(D3DRASTER_STATUS *pRasterStatus) {
    return Direct3DDevice8->GetRasterStatus(pRasterStatus);
}

HRESULT Direct3DDevice8Wrapper::GetLight(DWORD Index, D3DLIGHT8 *pLight) {
    return Direct3DDevice8->GetLight(Index, pLight);
}

HRESULT Direct3DDevice8Wrapper::GetLightEnable(DWORD Index, BOOL *pEnable) {
    return Direct3DDevice8->GetLightEnable(Index, pEnable);
}

HRESULT Direct3DDevice8Wrapper::GetMaterial(D3DMATERIAL8 *pMaterial) {
    return Direct3DDevice8->GetMaterial(pMaterial);
}

HRESULT Direct3DDevice8Wrapper::LightEnable(DWORD LightIndex, BOOL bEnable) {
    return Direct3DDevice8->LightEnable(LightIndex, bEnable);
}

HRESULT Direct3DDevice8Wrapper::SetLight(DWORD Index, CONST D3DLIGHT8 *pLight) {
    return Direct3DDevice8->SetLight(Index, pLight);
}

HRESULT Direct3DDevice8Wrapper::SetMaterial(CONST D3DMATERIAL8 *pMaterial) {
    return Direct3DDevice8->SetMaterial(pMaterial);
}

HRESULT Direct3DDevice8Wrapper::CopyRects(IDirect3DSurface8 *pSourceSurface, CONST RECT *pSourceRectsArray, UINT cRects, IDirect3DSurface8 *pDestinationSurface, CONST POINT *pDestPointsArray) {
    return Direct3DDevice8->CopyRects(pSourceSurface, pSourceRectsArray, cRects, pDestinationSurface, pDestPointsArray);
}

HRESULT Direct3DDevice8Wrapper::GetFrontBuffer(IDirect3DSurface8 *pDestSurface) {
    return Direct3DDevice8->GetFrontBuffer(pDestSurface);
}

HRESULT Direct3DDevice8Wrapper::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) {
    return Direct3DDevice8->MultiplyTransform(State, pMatrix);
}

HRESULT Direct3DDevice8Wrapper::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8 *pDestBuffer, DWORD Flags) {
    return Direct3DDevice8->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags);
}

HRESULT Direct3DDevice8Wrapper::ResourceManagerDiscardBytes(DWORD Bytes) {
    return Direct3DDevice8->ResourceManagerDiscardBytes(Bytes);
}

HRESULT Direct3DDevice8Wrapper::TestCooperativeLevel() {
    return Direct3DDevice8->TestCooperativeLevel();
}

HRESULT Direct3DDevice8Wrapper::GetCurrentTexturePalette(UINT *pPaletteNumber) {
    return Direct3DDevice8->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT Direct3DDevice8Wrapper::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries) {
    return Direct3DDevice8->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT Direct3DDevice8Wrapper::SetCurrentTexturePalette(UINT PaletteNumber) {
    return Direct3DDevice8->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT Direct3DDevice8Wrapper::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries) {
    return Direct3DDevice8->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT Direct3DDevice8Wrapper::CreatePixelShader(CONST DWORD *pFunction, DWORD *pHandle) {
    return Direct3DDevice8->CreatePixelShader(pFunction, pHandle);
}

HRESULT Direct3DDevice8Wrapper::DeletePixelShader(DWORD Handle) {
    return Direct3DDevice8->DeletePixelShader(Handle);
}

HRESULT Direct3DDevice8Wrapper::GetPixelShader(DWORD *pHandle) {
    return Direct3DDevice8->GetPixelShader(pHandle);
}

HRESULT Direct3DDevice8Wrapper::GetPixelShaderConstant(DWORD Register, void *pConstantData, DWORD ConstantCount) {
    return Direct3DDevice8->GetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT Direct3DDevice8Wrapper::GetPixelShaderFunction(DWORD Handle, void *pData, DWORD *pSizeOfData) {
    return Direct3DDevice8->GetPixelShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT Direct3DDevice8Wrapper::SetPixelShader(DWORD Handle) {
    return Direct3DDevice8->SetPixelShader(Handle);
}

HRESULT Direct3DDevice8Wrapper::SetPixelShaderConstant(DWORD Register, CONST void *pConstantData, DWORD ConstantCount) {
    return Direct3DDevice8->SetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

//HRESULT Direct3DDevice8Wrapper::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) {
//	return Direct3DDevice8->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
//}

//HRESULT Direct3DDevice8Wrapper::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) {
//	OutputDebugString("Device reset called.");
//	return Direct3DDevice8->Reset(PresentationParameters);
//}

HRESULT Direct3DDevice8Wrapper::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount) {
    return Direct3DDevice8->DrawIndexedPrimitive(Type, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}

HRESULT Direct3DDevice8Wrapper::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    return Direct3DDevice8->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT Direct3DDevice8Wrapper::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
    return Direct3DDevice8->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT Direct3DDevice8Wrapper::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) {
    return Direct3DDevice8->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT Direct3DDevice8Wrapper::BeginScene() {
    return Direct3DDevice8->BeginScene();
}

HRESULT Direct3DDevice8Wrapper::EndScene() {
    return Direct3DDevice8->EndScene();
}

HRESULT Direct3DDevice8Wrapper::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, UINT *pStride) {
    return Direct3DDevice8->GetStreamSource(StreamNumber, ppStreamData, pStride);
}

HRESULT Direct3DDevice8Wrapper::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8 *pStreamData, UINT Stride) {
    return Direct3DDevice8->SetStreamSource(StreamNumber, pStreamData, Stride);
}

HRESULT Direct3DDevice8Wrapper::GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer) {
    return Direct3DDevice8->GetBackBuffer(BackBuffer, Type, ppBackBuffer);
}

HRESULT Direct3DDevice8Wrapper::GetDepthStencilSurface(IDirect3DSurface8 **ppZStencilSurface) {
    return Direct3DDevice8->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT Direct3DDevice8Wrapper::GetTexture(DWORD Stage, IDirect3DBaseTexture8 **ppTexture) {
    return Direct3DDevice8->GetTexture(Stage, ppTexture);
}

HRESULT Direct3DDevice8Wrapper::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue) {
    return Direct3DDevice8->GetTextureStageState(Stage, Type, pValue);
}

HRESULT Direct3DDevice8Wrapper::SetTexture(DWORD Stage, IDirect3DBaseTexture8 *pTexture) {
    return Direct3DDevice8->SetTexture(Stage, pTexture);
}

HRESULT Direct3DDevice8Wrapper::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) {
    return Direct3DDevice8->SetTextureStageState(Stage, Type, Value);
}

HRESULT Direct3DDevice8Wrapper::UpdateTexture(IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture) {
    return Direct3DDevice8->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT Direct3DDevice8Wrapper::ValidateDevice(DWORD *pNumPasses) {
    return Direct3DDevice8->ValidateDevice(pNumPasses);
}

HRESULT Direct3DDevice8Wrapper::GetClipPlane(DWORD Index, float *pPlane) {
    return Direct3DDevice8->GetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice8Wrapper::SetClipPlane(DWORD Index, CONST float *pPlane) {
    return Direct3DDevice8->SetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice8Wrapper::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) {
    return Direct3DDevice8->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT Direct3DDevice8Wrapper::GetViewport(D3DVIEWPORT8 *pViewport) {
    return Direct3DDevice8->GetViewport(pViewport);
}

HRESULT Direct3DDevice8Wrapper::SetViewport(CONST D3DVIEWPORT8 *pViewport) {
    return Direct3DDevice8->SetViewport(pViewport);
}

HRESULT Direct3DDevice8Wrapper::CreateVertexShader(CONST DWORD *pDeclaration, CONST DWORD *pFunction, DWORD *pHandle, DWORD Usage) {
    return Direct3DDevice8->CreateVertexShader(pDeclaration, pFunction, pHandle, Usage);
}

HRESULT Direct3DDevice8Wrapper::DeleteVertexShader(DWORD Handle) {
    return Direct3DDevice8->DeleteVertexShader(Handle);
}

HRESULT Direct3DDevice8Wrapper::GetVertexShader(DWORD *pHandle) {
    return Direct3DDevice8->GetVertexShader(pHandle);
}

HRESULT Direct3DDevice8Wrapper::GetVertexShaderConstant(DWORD Register, void *pConstantData, DWORD ConstantCount) {
    return Direct3DDevice8->GetVertexShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT Direct3DDevice8Wrapper::GetVertexShaderDeclaration(DWORD Handle, void *pData, DWORD *pSizeOfData) {
    return Direct3DDevice8->GetVertexShaderDeclaration(Handle, pData, pSizeOfData);
}

HRESULT Direct3DDevice8Wrapper::GetVertexShaderFunction(DWORD Handle, void *pData, DWORD *pSizeOfData) {
    return Direct3DDevice8->GetVertexShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT Direct3DDevice8Wrapper::SetVertexShader(DWORD Handle) {
    return Direct3DDevice8->SetVertexShader(Handle);
}

HRESULT Direct3DDevice8Wrapper::SetVertexShaderConstant(DWORD Register, CONST void *pConstantData, DWORD ConstantCount) {
    return Direct3DDevice8->SetVertexShaderConstant(Register, pConstantData, ConstantCount);
}