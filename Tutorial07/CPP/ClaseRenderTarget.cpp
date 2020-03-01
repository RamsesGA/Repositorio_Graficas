#include "../Encabezados/ClaseRenderTarget.h"

void RenderTarget::Init(RenderTargetDesc _rtd) {

	m_RenderTDesc = _rtd;

#ifdef D3D11
	m_RenderTD3D11.ArraySize			= m_RenderTDesc.ArraySize;
	m_RenderTD3D11.BindFlags			= m_RenderTDesc.BindFlags;
	m_RenderTD3D11.CPUAccessFlags		= m_RenderTDesc.CPUAccessFlags;
	m_RenderTD3D11.Format				= (DXGI_FORMAT)m_RenderTDesc.Format;
	m_RenderTD3D11.Height				= m_RenderTDesc.Height;
	m_RenderTD3D11.MipLevels			= m_RenderTDesc.MipLevels;
	m_RenderTD3D11.MiscFlags			= m_RenderTDesc.MiscFlags;
	m_RenderTD3D11.SampleDesc.Count		= m_RenderTDesc.SampleDesc.My_Count;
	m_RenderTD3D11.SampleDesc.Quality	= m_RenderTDesc.SampleDesc.My_Quality;
	m_RenderTD3D11.Usage				= (D3D11_USAGE)m_RenderTDesc.Usage;
	m_RenderTD3D11.Width				= m_RenderTDesc.Width;
#endif
}
