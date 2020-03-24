#include "..\Encabezados\ClaseSwapChain.h"

///
/// function to initialize swapchain values
///
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

///
/// function to get swap data
///
void* ClaseSwapChain::GetSwap()
{
#if defined(D3D11)
	return  g_pSwapChainD3D11;
#endif
	return nullptr;
}