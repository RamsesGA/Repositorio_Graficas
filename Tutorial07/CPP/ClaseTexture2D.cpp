#include "../Encabezados/ClaseTexture2D.h"

void ClaseTextura2D::Init(Texture2Desc _text){

	m_TextDesc = _text;

#ifdef D3D11
	ZeroMemory(&m_TextDescD3D11, sizeof(m_TextDescD3D11));

	m_TextDescD3D11.Width				= m_TextDesc.Width;
	m_TextDescD3D11.Height				= m_TextDesc.Height;
	m_TextDescD3D11.MipLevels			= m_TextDesc.MipLevels;
	m_TextDescD3D11.ArraySize			= m_TextDesc.ArraySize;
	m_TextDescD3D11.Format				= (DXGI_FORMAT)m_TextDesc.Format;
	m_TextDescD3D11.SampleDesc.Count	= m_TextDesc.SampleDesc.My_Count;
	m_TextDescD3D11.SampleDesc.Quality	= m_TextDesc.SampleDesc.My_Quality;
	m_TextDescD3D11.Usage				= (D3D11_USAGE)m_TextDesc.Usage;
	m_TextDescD3D11.BindFlags			= m_TextDesc.BindFlags;
	m_TextDescD3D11.CPUAccessFlags		= m_TextDesc.CPUAccessFlags;
	m_TextDescD3D11.MiscFlags			= m_TextDesc.MiscFlags;
#endif
}

void* ClaseTextura2D::GetDepth()
{
#if defined(D3D11)
	return  m_TextureD3D11;
#endif
	return nullptr;
}
