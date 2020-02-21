#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

struct DeviceContextDescriptor {

};

class ClaseDeviceContext {

	public:

		//
		DeviceContextDescriptor m_DescDCont;

		//
		ClaseDeviceContext() {};
		~ClaseDeviceContext() {};

		//
		void
		Init(ID3D11DeviceContext* _context);
		//
		void
		Update();
		//
		void
		Render();
		//
		void
		Delete();

		/*//
		void
		SetDeviceContext(ID3D11DeviceContext* _context);
		ID3D11DeviceContext*
		GetDeviceContext() { return m_DescDCont.g_pImmediateContext; };*/

		/*//Abstract
		void
		OMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews, __in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView);

		void
		RSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports, __in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports);

		void 
		UpdateSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);

		void 
		IASetInputLayout(ID3D11InputLayout* pInputLayout);

		void 
		IASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,__in_ecount(NumBuffers)  ID3D11Buffer* const* ppVertexBuffers,__in_ecount(NumBuffers)  const UINT* pStrides,__in_ecount(NumBuffers)  const UINT* pOffsets);

		void 
		IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,DXGI_FORMAT Format, UINT Offset);

		void
		IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

		void 
		ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

		void 
		ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil);

		void 
		VSSetShader(ID3D11VertexShader* pVertexShader,__in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);

		void 
		VSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers, __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

		void 
		PSSetShader(ID3D11PixelShader* pPixelShader, __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);

		void 
		PSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers, __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

		void 
		PSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews, __in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceView);

		void 
		PSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot, __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers, __in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers);

		void 
		DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);*/

#ifdef D3D11
		ID3D11DeviceContext* g_pImmediateContextD3D11;
#endif // D3D11
};