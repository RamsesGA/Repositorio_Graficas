#include "../Encabezados/ClaseDepthStencil.h"

///
/// function to initialize depth stencil values
///
void ClaseDepthStencil::Init(DepthStencilViewDesc _stencilDesc){

    m_DepthDesc = _stencilDesc;
#ifdef D3D11
	ZeroMemory(&descDepthViewD3D11, sizeof(descDepthViewD3D11));

   descDepthViewD3D11.Format                = (DXGI_FORMAT)m_DepthDesc.Format;
   //descDepthViewD3D11.Format                = _format;
   descDepthViewD3D11.Flags                 = m_DepthDesc.Flags;
   descDepthViewD3D11.ViewDimension         = (D3D11_DSV_DIMENSION)m_DepthDesc.ViewDimension;
   descDepthViewD3D11.Texture2D.MipSlice    = m_DepthDesc.Texture2D.My_MipSlice;
#endif
}
