#include "../Encabezados/ClaseDepthStencil.h"

void ClaseDepthStencil::Init(DepthStencilDesc _stencilDesc){

	m_DepthDesc = _stencilDesc;
#ifdef D3D11
	ZeroMemory(&descDepthD3D11, sizeof(descDepthD3D11));

   descDepthD3D11.Width                     = m_DepthDesc.Width;
   descDepthD3D11.Height                    = m_DepthDesc.Height;
   descDepthD3D11.MipLevels                 = m_DepthDesc.MipLevels;
   descDepthD3D11.ArraySize                 = m_DepthDesc.ArraySize;
   descDepthD3D11.Format                    = (DXGI_FORMAT)m_DepthDesc.Format;
   descDepthD3D11.SampleDesc.Count          = m_DepthDesc.SampleDesc.My_Count;
   descDepthD3D11.SampleDesc.Quality        = m_DepthDesc.SampleDesc.My_Quality;
   descDepthD3D11.Usage                     = (D3D11_USAGE)m_DepthDesc.Usage;
   descDepthD3D11.BindFlags                 = m_DepthDesc.BindFlags;
   descDepthD3D11.CPUAccessFlags            = m_DepthDesc.CPUAccessFlags;
   descDepthD3D11.MiscFlags                 = m_DepthDesc.MiscFlags;
#endif
}
