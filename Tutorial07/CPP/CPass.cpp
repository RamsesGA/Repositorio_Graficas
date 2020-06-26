#include "..\Encabezados\CPass.h"

#ifdef OPENGL

void CPass::Pass(PassData& _sPassData){

    glBindFramebuffer(GL_FRAMEBUFFER, _sPassData.s_RenderTarget.m_IdRenderTarget);
    glUseProgram(_sPassData.m_programShaderID);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mathfu::float4x4 word = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    mathfu::float4x4 view = _sPassData.s_NeverChanges.mView;
    view = view.Transpose();
    mathfu::float4x4 projection = _sPassData.s_ChangeOR.mProjection;
    projection = projection.Transpose();

    /// word matrix
    GLuint MatrixID = glGetUniformLocation(_sPassData.m_programShaderID, "WordMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &word[0]);

    /// view matrix
    MatrixID = glGetUniformLocation(_sPassData.m_programShaderID, "ViewMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0]);

    /// projection matrix
    MatrixID = glGetUniformLocation(_sPassData.m_programShaderID, "ProjectionMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projection[0]);

    //Dirección de luz
    GLuint idLight = glGetUniformLocation(_sPassData.m_programShaderID, "LightDir");
    float DirLight[4] = { _sPassData.s_Lights.mLightDir[0], _sPassData.s_Lights.mLightDir[1], _sPassData.s_Lights.mLightDir[2], 0 };
    glUniform4fv(idLight, 1, &DirLight[0]);

    //Aquí debo agregar lo nuevo

    for (int i = 0; i < _sPassData.s_Mesh[0].size(); i++) {

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, _sPassData.s_Mesh[0][i]->GetVertexBuffer()->m_ID);

        glVertexAttribPointer
        (
            /// attribute 0. No particular reason for 0, but must match the layout in the shader.
            0,
            /// size
            3,
            /// type
            GL_FLOAT,
            /// normalized?
            GL_FALSE,
            /// stride
            sizeof(SimpleVertex),
            /// array buffer offset
            (void*)0
        );

        glBindBuffer(GL_ARRAY_BUFFER, _sPassData.s_Mesh[0][i]->GetVertexBuffer()->m_ID);
        glVertexAttribPointer
        (
            /// attribute 0. No particular reason for 0, but must match the layout in the shader.
            1,
            /// size
            3,
            /// type
            GL_FLOAT,
            /// normalized?
            GL_FALSE,
            /// stride
            sizeof(SimpleVertex),
            /// array buffer offset, va a tomar del 0 + 3 por el tamaño del flotante
            (unsigned char*)NULL + (3 * sizeof(float))
        );

        glVertexAttribPointer
        (
            /// attribute 0. No particular reason for 0, but must match the layout in the shader.
            2,
            /// size
            2,
            /// type
            GL_FLOAT,
            /// normalized?
            GL_FALSE,
            /// stride
            sizeof(SimpleVertex),
            /// array buffer offset
            (unsigned char*)NULL + (6 * sizeof(float))
        );

        /// Draw the triangle
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sPassData.s_Mesh[0][i]->GetIndexBuffer()->m_ID);

        ///1st attribute buffer : vertices
        GLuint texID = glGetUniformLocation(_sPassData.m_programShaderID, "textureDifuse");

        glUniform1i(texID, _sPassData.s_Mesh[0][i]->m_Materials->m_TextureId);

        glActiveTexture(GL_TEXTURE0 + _sPassData.s_Mesh[0][i]->m_Materials->m_TextureId);

        glBindTexture(GL_TEXTURE_2D, _sPassData.s_Mesh[0][i]->m_Materials->m_TextureId);

        ///Draw the triangles
        glDrawElements
        (
            ///mode
            GL_TRIANGLES,
            ///count
            _sPassData.s_Mesh[0][i]->GetIndexNum(),
            ///type
            GL_UNSIGNED_SHORT,
            ///element array buffer offset
            (void*)0
        );

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }
}
#endif

#ifdef D3D11
void CPass::Render(ClaseRenderTargetView& _renderTargView, ClaseDepthStencil& _depthStencil, ClaseDeviceContext* _devContext, SCENEMANAGER& _sceneManager, Camera* _camera, ClaseBuffer* _light){

    // render target
    {
        _devContext->g_pImmediateContextD3D11->OMSetRenderTargets(1, &_renderTargView.g_pRenderTargetViewD3D11, _depthStencil.g_pDepthStencilViewD3D11);
    }

    // shaders
    {
        // vertex
        _devContext->g_pImmediateContextD3D11->IASetInputLayout(m_PassDX.s_InputLayout->LayoutD3D11);
        _devContext->g_pImmediateContextD3D11->VSSetShader(m_PassDX.s_VertexShader->g_pVertexShaderD3D11, NULL, 0);

        // fragment
        _devContext->g_pImmediateContextD3D11->PSSetShader(m_PassDX.s_PixelShader->pPixelShader, NULL, 0);
    }

    // misc config
    {
        // viewport
        _devContext->g_pImmediateContextD3D11->RSSetViewports(1, &m_PassDX.s_ViewPort->vpD3D11);
    }

    //Agregamos esto como extra
    _devContext->g_pImmediateContextD3D11->VSSetConstantBuffers(0, 1, &_camera->g_pCBNeverChangesCamera.m_BufferD3D11);
    _devContext->g_pImmediateContextD3D11->VSSetConstantBuffers(1, 1, &_camera->g_pCBChangeOnResizeCamera.m_BufferD3D11);


    // al final de render en el pase!!!!!
    for (int i = 0; i < _sceneManager.m_MeshInScene.size(); i++)
    {

        // constant buffers
        {
            _devContext->g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &_camera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
            _devContext->g_pImmediateContextD3D11->PSSetConstantBuffers(2, 1, &_camera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
            _devContext->g_pImmediateContextD3D11->PSSetConstantBuffers(3, 1, &_light->m_BufferD3D11);
        }

        // mesh
        {
            // Set vertex buffer
            UINT stride = sizeof(SimpleVertex);
            UINT offset = 0;
            _devContext->g_pImmediateContextD3D11->IASetVertexBuffers(0, 1, &_sceneManager.m_MeshInScene[i]->m_VertexBuffer->m_BufferD3D11, &stride, &offset);

            // index buffer
            _devContext->g_pImmediateContextD3D11->IASetIndexBuffer(_sceneManager.m_MeshInScene[i]->m_Index->m_BufferD3D11, DXGI_FORMAT_R16_UINT, 0);

            // Set primitive topology
            _devContext->g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            // material
            _devContext->g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &_sceneManager.m_MeshInScene[i]->m_Materials->m_TexDif);

            _devContext->g_pImmediateContextD3D11->DrawIndexed(_sceneManager.m_MeshInScene[i]->GetIndexNum(), 0, 0);
        }
    }
}

int CPass::Init(PassDX& _struct){

#ifdef D3D11
    m_PassDX.s_InputLayout = _struct.s_InputLayout;
    m_PassDX.s_PixelShader = _struct.s_PixelShader;
    m_PassDX.s_VertexShader = _struct.s_VertexShader;
    m_PassDX.s_ViewPort = _struct.s_ViewPort;
#endif
    return 0;
}
#endif