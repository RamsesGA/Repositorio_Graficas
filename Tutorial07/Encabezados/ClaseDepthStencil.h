#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

struct DepthStencilDesc {

	TEXTURE2D_DESC descDepth;

	UINT Width;
	UINT Height;
	UINT MipLevels;
	UINT ArraySize;
	FORMAT Format;
	SAMPLE_DESC SampleDesc;
	USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
};

class ClaseDepthStencil {

	public:
		ClaseDepthStencil() {};
		~ClaseDepthStencil() {};

		DepthStencilDesc m_DepthDesc;

		void
		Init(DepthStencilDesc _stencilDesc);

		void
		Render();

		void
		Update();

		void
		Delete();

#ifdef D3D11
		ID3D11DepthStencilView* g_pDepthStencilViewD3D11;
		D3D11_TEXTURE2D_DESC descDepthD3D11;
#endif // D3D11
};