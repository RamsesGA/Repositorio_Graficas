#pragma once
#include "Defines.h"

///
/// Render Target Descriptor
///
struct RenderTargetDesc {

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
/// Class with the Render Target abstraction
///
class RenderTarget {

	public:
        RenderTarget() {};
        ~RenderTarget() {};

        /// OPENGL´S memebers
#ifdef OPENGL
        GLuint m_IdRenderTarget = 0;
#endif // OPENGL

        bool m_initialize = false;
        RenderTargetDesc m_RenderTDesc;

        /// OPENGL´S methods
        void
        Init(RenderTargetDesc _rtd);

#ifdef OPENGL
        void
        InitFrameBuffer();
#endif // OPENGL

        /// DIRECTX´S memebers
#ifdef D3D11
        ID3D11Texture2D* g_pDepthStencilD3D11;
        D3D11_TEXTURE2D_DESC m_RenderTD3D11;
#endif // D3D11
};