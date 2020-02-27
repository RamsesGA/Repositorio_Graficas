#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

struct Texture2Desc {

#ifdef D3D11
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
#endif
};

class ClaseTextura2D {

	public:
		ClaseTextura2D() {
#ifdef D3D11
            m_TextureD3D11 = NULL;
#endif // D3D11
        };
		~ClaseTextura2D() {};

        Texture2Desc m_TextDesc;

		void
		Init(Texture2Desc _text);

#ifdef D3D11
        ID3D11Texture2D* m_TextureD3D11;
        D3D11_TEXTURE2D_DESC m_TextDescD3D11;
#endif // D3D11
};