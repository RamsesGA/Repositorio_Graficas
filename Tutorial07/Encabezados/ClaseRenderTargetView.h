#pragma once
#include "Defines.h"

///
/// Render descriptor target view
///
struct RenderTargetViewDesc {

	FORMAT Format;
	RTV_DIMENSION ViewDimension;
    BUFFER_RTV Buffer;
    TEX1D_RTV Texture1D;
    TEX1D_ARRAY_RTV Texture1DArray;
    TEX2D_RTV Texture2D;
    TEX2D_ARRAY_RTV Texture2DArray;
    TEX2DMS_RTV Texture2DMS;
    TEX2DMS_ARRAY_RTV Texture2DMSArray;
    TEX3D_RTV Texture3D;
};

///
/// Class with the Render Target View abstraction
///
class ClaseRenderTargetView {

	public:

        ///
        /// Builder and destroyer
        ///
		ClaseRenderTargetView() {};
		~ClaseRenderTargetView() {};

        RenderTargetViewDesc m_renderTVD;

        ///
        /// OpenGL members
        ///
#ifdef OPENGL
        GLuint m_IdRenderTargetView = 0;
#endif // OPENGL
        
        ///
        /// Methods
        ///
        void
        Init(RenderTargetViewDesc _rtvd);

        ///
        /// DIRECTX members
        ///
#ifdef D3D11
        ID3D11RenderTargetView* g_pRenderTargetViewD3D11;
        D3D11_RENDER_TARGET_VIEW_DESC m_renderTVD3D11;
#endif // D3D11
};