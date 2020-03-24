#include "../Encabezados/ClaseRenderTarget.h"

///
/// function to initialize the Render target values
///
void RenderTarget::Init(RenderTargetDesc _rtd) {

	m_RenderTDesc = _rtd;

#ifdef D3D11
	m_RenderTD3D11.ArraySize			= m_RenderTDesc.ArraySize;
	m_RenderTD3D11.BindFlags			= m_RenderTDesc.BindFlags;
	m_RenderTD3D11.CPUAccessFlags		= m_RenderTDesc.CPUAccessFlags;
	m_RenderTD3D11.Format				= (DXGI_FORMAT)m_RenderTDesc.Format;
	m_RenderTD3D11.Height				= m_RenderTDesc.Height;
	m_RenderTD3D11.MipLevels			= m_RenderTDesc.MipLevels;
	m_RenderTD3D11.MiscFlags			= m_RenderTDesc.MiscFlags;
	m_RenderTD3D11.SampleDesc.Count		= m_RenderTDesc.SampleDesc.My_Count;
	m_RenderTD3D11.SampleDesc.Quality	= m_RenderTDesc.SampleDesc.My_Quality;
	m_RenderTD3D11.Usage				= (D3D11_USAGE)m_RenderTDesc.Usage;
	m_RenderTD3D11.Width				= m_RenderTDesc.Width;
#endif


}

///
/// function to initialize Frame Buffer values
///
void RenderTarget::InitFrameBuffer(){

    if (m_initialize) {

        return;
    }

    glGenFramebuffers(1, &m_IdRenderTarget);
    glBindFramebuffer(GL_FRAMEBUFFER, m_IdRenderTarget);

    ///
    /// The texture we're going to render to
    ///
    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);

    ///
    /// "Bind" the newly created texture : all future texture functions will modify this texture
    ///
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    ///
    /// Give an empty image to OpenGL ( the last "0" )
    ///
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    ///
    /// Poor filtering. Needed !
    ///
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    ///
    /// The depth buffer
    ///
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    ///
    /// Set "renderedTexture" as our colour attachement #0
    ///
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    ///
    /// Set the list of draw buffers.
    ///
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

    ///
    /// "1" is the size of DrawBuffers
    ///
    glDrawBuffers(1, DrawBuffers);

    ///
    /// Always check that our framebuffer is ok
    ///
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {}
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_initialize = true;
}
