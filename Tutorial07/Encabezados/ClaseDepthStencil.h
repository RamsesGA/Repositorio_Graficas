#pragma once
#include "Defines.h"

///
/// Depth Stencil View Descriptor
///
struct DepthStencilViewDesc {
	FORMAT Format;
	DSV_DIMENSION ViewDimension;
	UINT Flags;
	TEX2D_DSV Texture2D;
};

///
/// Depth Stencil Functions Class
///
class ClaseDepthStencil {

	public:
		ClaseDepthStencil() {};
		~ClaseDepthStencil() {};

		DepthStencilViewDesc m_DepthDesc;

		void
		Init(DepthStencilViewDesc _stencilDesc);

		void
		Render();

		void
		Update();

		void
		Delete();

#ifdef D3D11
		ID3D11DepthStencilView* g_pDepthStencilViewD3D11;
		D3D11_DEPTH_STENCIL_VIEW_DESC descDepthViewD3D11;
#endif // D3D11
};