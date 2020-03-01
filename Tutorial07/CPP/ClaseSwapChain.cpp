#include "..\Encabezados\ClaseSwapChain.h"

void ClaseSwapChain::Init(SwapChainDescriptor _desc){

	m_ChainDesc = _desc;

#ifdef D3D11
	ZeroMemory(&sdD3D11, sizeof(sdD3D11));
	
	sdD3D11.BufferCount							= m_ChainDesc.BufferCount;
	sdD3D11.BufferDesc.Width					= m_ChainDesc.BufferDesc.My_Width;
	sdD3D11.BufferDesc.Height					= m_ChainDesc.BufferDesc.My_Height;
	sdD3D11.BufferDesc.Format					= (DXGI_FORMAT)m_ChainDesc.BufferDesc.My_Format;
	sdD3D11.BufferDesc.RefreshRate.Numerator	= m_ChainDesc.BufferDesc.My_RefreshRate.My_Numerator;
	sdD3D11.BufferDesc.RefreshRate.Denominator	= m_ChainDesc.BufferDesc.My_RefreshRate.My_Denominator;
	sdD3D11.BufferUsage							= m_ChainDesc.BufferUsage;
	sdD3D11.OutputWindow						= m_ChainDesc.OutputWindow;
	sdD3D11.SampleDesc.Count					= m_ChainDesc.SampleDesc.My_Count;
	sdD3D11.SampleDesc.Quality					= m_ChainDesc.SampleDesc.My_Quality;
	sdD3D11.Windowed							= m_ChainDesc.Windowed;
#endif
}

/*void ClaseSwapChain::SetSwapChain(IDXGISwapChain* _chain){

	m_ChainDesc.g_pSwapChain = _chain;
}

void ClaseSwapChain::SetMode(DXGI_MODE_DESC _buffer){

	m_ChainDesc.BufferDesc = _buffer;
}

void ClaseSwapChain::SetSample(DXGI_SAMPLE_DESC _sample){

	m_ChainDesc.SampleDesc = _sample;
}

void ClaseSwapChain::SetUsage(DXGI_USAGE _usage){

	m_ChainDesc.BufferUsage = _usage;
}

void ClaseSwapChain::SetBufferC(UINT _int){

	m_ChainDesc.BufferCount = _int;
}

void ClaseSwapChain::SetWindow(HWND _hwnd){

	m_ChainDesc.OutputWindow = _hwnd;
}

void ClaseSwapChain::SetWindowed(BOOL _bool){

	m_ChainDesc.Windowed = _bool;
}

void ClaseSwapChain::SetSwap(DXGI_SWAP_EFFECT _swapEffect){

	m_ChainDesc.SwapEffect = _swapEffect;
}

void ClaseSwapChain::SetFlags(UINT _int){

	m_ChainDesc.Flags = _int;
}

HRESULT ClaseSwapChain::ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags){

	return E_NOTIMPL;
}

HRESULT ClaseSwapChain::GetBackBuffer(UINT Buffer, REFIID riid, void** ppSurface){

	return E_NOTIMPL;
}

HRESULT ClaseSwapChain::Present(UINT SyncInterval, UINT Flags){

	return E_NOTIMPL;
}*/