#include "..\Encabezados\ClaseDevice.h"

//Main functions
void ClaseDevice::Init(DeviceDescriptor _devDesc){

	m_DescDevice = _devDesc;
}

void ClaseDevice::Update(){
}

void ClaseDevice::Render(){
}

void ClaseDevice::Delete(){
}

void* ClaseDevice::GetDev(){

#if defined(D3D11)
	return  g_pd3dDeviceD3D11;
#endif
	return nullptr;
}

/*//-----
HRESULT ClaseDevice::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView){

	return E_NOTIMPL;
}

HRESULT ClaseDevice::CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, __in_xcount_opt(pDesc->MipLevels* pDesc->ArraySize)  const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D) {

	return E_NOTIMPL;
}

HRESULT ClaseDevice::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView) {

	return E_NOTIMPL;
}

HRESULT ClaseDevice::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader){

	return E_NOTIMPL;
}

HRESULT ClaseDevice::CreateInputLayout(__in_ecount(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout){

	return E_NOTIMPL;
}

HRESULT ClaseDevice::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader){

	return E_NOTIMPL;
}

HRESULT ClaseDevice::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer){

	return E_NOTIMPL;
}

HRESULT ClaseDevice::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState){

	return E_NOTIMPL;
}*/