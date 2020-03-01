#pragma once
#include "Defines.h"

struct DeviceDescriptor {

	UINT s_createDeviceFlags;
	UINT s_numFeatureLevels;

	DRIVER_TYPE		g_driverType;
	FEATURE_LEVEL	g_featureLevel;
};

class ClaseDevice {

	public:
		
		DeviceDescriptor m_DescDevice;
		
		//
		ClaseDevice() {};
		~ClaseDevice() {};

		/*//D3D_DRIVER_TYPE
		void
		SetDriverType(D3D_DRIVER_TYPE _dType);
		D3D_DRIVER_TYPE
		GetDriverType() { return m_DescDevice.g_driverType; };

		//D3D_FEATURE_LEVEL
		void 
		SetFeatureLevel(D3D_FEATURE_LEVEL _dType);
		D3D_FEATURE_LEVEL
		GetFeatureLevel() { return m_DescDevice.g_featureLevel; };

		//ID3D11Device
		void
		SetDevice(ID3D11Device* _device);
		ID3D11Device* 
		GetDevice() { return m_DescDevice.g_pd3dDevice; };*/

		//
		void
		Init(DeviceDescriptor _devDesc);
		void
		Update();
		void
		Render();
		void
		Delete();

		/*//Abstraction
		HRESULT 
		CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);
		HRESULT
		CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, __in_xcount_opt(pDesc->MipLevels* pDesc->ArraySize)  const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
		HRESULT
		CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView);
		HRESULT
		CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader);
		HRESULT
		CreateInputLayout(__in_ecount(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout);
		HRESULT
		CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader);
		HRESULT
		CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);
		HRESULT
		CreateSamplerState( const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);*/

#ifdef D3D11
		ID3D11Device*		g_pd3dDeviceD3D11;
		D3D_DRIVER_TYPE		g_driverTypeD3D11;
		D3D_FEATURE_LEVEL	g_featureLevelD3D11;
		
#endif // D3D11
};