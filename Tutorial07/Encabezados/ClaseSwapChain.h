#pragma once
#include "Defines.h"

///
/// Swap Chain Descriptor
///
struct SwapChainDescriptor {

	SWAP_CHAIN_DESC		sd;

	MODE_DESC			BufferDesc;
	SAMPLE_DESC			SampleDesc;
	UINT				BufferUsage;
	UINT				BufferCount;
	HWND				OutputWindow;
	BOOL				Windowed;
	SWAP_EFFECT			SwapEffect;
	UINT				Flags;
	
	UINT				s_widht;
	UINT				s_height;
	UINT				s_numerator;
	UINT				s_denominator;
	UINT				s_count;
	UINT				s_quality;
};

///
/// Swap Chain Abstraction Class
///
class ClaseSwapChain {

	public:
		ClaseSwapChain() {};
		~ClaseSwapChain() {};

		SwapChainDescriptor m_ChainDesc;

		void
		Init(SwapChainDescriptor _desc);

		void*
		GetSwap();
		
#ifdef D3D11
		IDXGISwapChain* g_pSwapChainD3D11;
		DXGI_SWAP_CHAIN_DESC sdD3D11;
#endif // D3D11
};