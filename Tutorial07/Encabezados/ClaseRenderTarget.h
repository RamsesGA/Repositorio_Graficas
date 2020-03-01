#pragma once
#include "Defines.h"

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

class RenderTarget {

	public:
        RenderTarget() {};
        ~RenderTarget() {};

        RenderTargetDesc m_RenderTDesc;

        void
        Init(RenderTargetDesc _rtd);

#ifdef D3D11
        ID3D11Texture2D* g_pDepthStencilD3D11;
        D3D11_TEXTURE2D_DESC m_RenderTD3D11;
#endif // D3D11
};