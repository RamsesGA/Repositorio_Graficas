#include "../Encabezados/ClaseViewport.h"

///
/// function to initialize viewport values
///
void ClaseViewport::Init(ViewportDesc _viewDesc){

	m_ViewDesc = _viewDesc;

#ifdef D3D11
	vpD3D11.Height =	m_ViewDesc.Height;
	vpD3D11.Width =		m_ViewDesc.Width;
	vpD3D11.MaxDepth =	m_ViewDesc.MaxDepth;
	vpD3D11.MinDepth =	m_ViewDesc.MinDepth;
	vpD3D11.TopLeftX =	m_ViewDesc.TopLeftX;
	vpD3D11.TopLeftY =	m_ViewDesc.TopLeftY;
#endif
}