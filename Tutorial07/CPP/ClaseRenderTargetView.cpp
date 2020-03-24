#include "../Encabezados/ClaseRenderTargetView.h"

///
/// function to initialize render values target view
///
void ClaseRenderTargetView::Init(RenderTargetViewDesc _rtvd){

	m_renderTVD = _rtvd;

#ifdef D3D11
	ZeroMemory(&m_renderTVD3D11, sizeof(m_renderTVD3D11));
	m_renderTVD3D11.Buffer.ElementOffset						= m_renderTVD.Buffer.My_ElementOffset;
	m_renderTVD3D11.Buffer.ElementWidth							= m_renderTVD.Buffer.My_ElementWidth;
	m_renderTVD3D11.Buffer.FirstElement							= m_renderTVD.Buffer.My_FirstElement;
	m_renderTVD3D11.Buffer.NumElements							= m_renderTVD.Buffer.My_NumElements;
	m_renderTVD3D11.Format										= (DXGI_FORMAT)m_renderTVD.Format;
	m_renderTVD3D11.Texture1D.MipSlice							= m_renderTVD.Texture1D.My_MipSlice;
	m_renderTVD3D11.Texture1DArray.ArraySize					= m_renderTVD.Texture1DArray.My_ArraySize;
	m_renderTVD3D11.Texture1DArray.FirstArraySlice				= m_renderTVD.Texture1DArray.My_FirstArraySlice;
	m_renderTVD3D11.Texture1DArray.MipSlice						= m_renderTVD.Texture1DArray.My_MipSlice;
	m_renderTVD3D11.Texture2D.MipSlice							= m_renderTVD.Texture2D.My_MipSlice;
	m_renderTVD3D11.Texture2DArray.ArraySize					= m_renderTVD.Texture2DArray.My_ArraySize;
	m_renderTVD3D11.Texture2DArray.FirstArraySlice				= m_renderTVD.Texture2DArray.My_FirstArraySlice;
	m_renderTVD3D11.Texture2DArray.MipSlice						= m_renderTVD.Texture2DArray.My_MipSlice;
	m_renderTVD3D11.Texture2DMS.UnusedField_NothingToDefine		= m_renderTVD.Texture2DMS.My_UnusedField_NothingToDefine;
	m_renderTVD3D11.Texture2DMSArray.ArraySize					= m_renderTVD.Texture2DMSArray.My_ArraySize;
	m_renderTVD3D11.Texture2DMSArray.FirstArraySlice			= m_renderTVD.Texture2DMSArray.My_FirstArraySlice;
	m_renderTVD3D11.Texture3D.FirstWSlice						= m_renderTVD.Texture3D.My_FirstWSlice;
	m_renderTVD3D11.Texture3D.MipSlice							= m_renderTVD.Texture3D.My_MipSlice;
	m_renderTVD3D11.Texture3D.WSize								= m_renderTVD.Texture3D.My_WSize;
	m_renderTVD3D11.ViewDimension								= (D3D11_RTV_DIMENSION)m_renderTVD.ViewDimension;
#endif
}
