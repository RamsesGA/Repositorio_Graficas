#pragma once
#include "Defines.h"

///
/// Viewport descriptor
///
struct ViewportDesc {

	VIEWPORT vp;

	FLOAT Width;
	FLOAT Height;
	FLOAT MinDepth;
	FLOAT MaxDepth;
	FLOAT TopLeftX;
	FLOAT TopLeftY;
};

///
/// Class with Viewport abstraction
///
class ClaseViewport {

	public:
		ClaseViewport() {};
		~ClaseViewport() {};

		ViewportDesc m_ViewDesc;

		void 
		Init(ViewportDesc _viewDesc);

		void
		Update();

		void
		Render();

		void
		Destroy();

#ifdef D3D11
		D3D11_VIEWPORT vpD3D11;
#endif // D3D11
};