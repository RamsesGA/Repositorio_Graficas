#pragma once
#include "Defines.h"

///
/// 2D Texture Descriptor
///
struct Texture2Desc {

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

///
/// Class with 2D Texture abstraction
///
class ClaseTextura2D {

	public:

        ///
        /// Builder and destroyer
        ///
		ClaseTextura2D() {
#ifdef D3D11
            m_TextureD3D11 = NULL;
#endif // D3D11
        };
		~ClaseTextura2D() {};

        ///
        /// Members
        ///
        Texture2Desc m_TextDesc;

        ///
        /// Main methods for the class
        ///
		void
		Init(Texture2Desc _text);

        void*
        GetDepth();

        ///
        /// DIRECTX members
        ///
#ifdef D3D11
        ID3D11Texture2D* m_TextureD3D11;
        D3D11_TEXTURE2D_DESC m_TextDescD3D11;
#endif // D3D11
};