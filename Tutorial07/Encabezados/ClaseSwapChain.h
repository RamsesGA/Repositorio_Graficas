#pragma once
#include "Defines.h"

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

class ClaseSwapChain {

	public:
		ClaseSwapChain() {};
		~ClaseSwapChain() {};

		SwapChainDescriptor m_ChainDesc;

		void
		Init(SwapChainDescriptor _desc);

		void*
		GetSwap();

		/*//
		void 
		SetSwapChain(IDXGISwapChain* _chain);
		IDXGISwapChain*
		GetSwapChain() { return m_ChainDesc.g_pSwapChain; };

		//
		void 
		SetMode(DXGI_MODE_DESC _buffer);
		DXGI_MODE_DESC
		GetMode() { return m_ChainDesc.BufferDesc; };

		//
		void 
		SetSample(DXGI_SAMPLE_DESC _sample);
		DXGI_SAMPLE_DESC
		GetSample() { return m_ChainDesc.SampleDesc; };

		//
		void 
		SetUsage(DXGI_USAGE _usage);
		DXGI_USAGE
		GetUsage() { return m_ChainDesc.BufferUsage; };

		//
		void 
		SetBufferC(UINT _int);
		UINT 
		GetBufferC() { return m_ChainDesc.BufferCount; };

		//
		void 
		SetWindow(HWND _hwnd);
		HWND
		GetWindow() { return m_ChainDesc.OutputWindow; };

		//
		void 
		SetWindowed(BOOL _bool);
		BOOL
		GetWindowed() { return m_ChainDesc.Windowed; };

		//
		void 
		SetSwap(DXGI_SWAP_EFFECT _swapEffect);
		DXGI_SWAP_EFFECT
		GetSwap() { return m_ChainDesc.SwapEffect; };

		//
		void 
		SetFlags(UINT _int);
		UINT
		GetFlags() { return m_ChainDesc.Flags; };


		//Abstract
		HRESULT 
		ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

		HRESULT 
		GetBackBuffer(UINT Buffer, REFIID riid, void** ppSurface);

		HRESULT
		Present(UINT SyncInterval, UINT Flags);*/

#ifdef D3D11
		IDXGISwapChain* g_pSwapChainD3D11;
		DXGI_SWAP_CHAIN_DESC sdD3D11;
#endif // D3D11
};