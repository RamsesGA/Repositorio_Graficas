#include "Encabezados/ClaseOpenGL.h"
#include "Encabezados/ClaseShader.h"
#include <iostream>

///
/// Global variables
///
Camera* g_CurrentCamera;

///
/// The callback function receives the new size of the framebuffer when it is resized, which can for example be used to update the OpenGL viewport.
///
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    
    glViewport(0, 0, width, height);
}

///
/// Initialization function
///
void ClaseOpenGL::InitDevice(){

    //FrameBuffer();
    m_RenderTarget.InitFrameBuffer();

    ///
    /// Initialization of the models
    ///
    m_SceneManager.Init();

    ///
    /// Loading models
    ///
    //m_GraphicApi.ChargeMesh("ugandan-knuckles/source/Knuckles.fbx", m_GraphicApi.m_Model, &m_SceneManager);
    m_GraphicApi.ChargeMesh("EscenaDelMaestro/Model/Scene/Scene.fbx", m_GraphicApi.m_Model, &m_SceneManager);

    ///
    /// Load vertex and pixel shader
    ///
    m_programShaderID = ClaseShader::LoadShaders("GBufferVS.glsl", "GBufferPS.glsl");
    glUseProgram(m_programShaderID);

    m_Buffers.OpenGLVAO();

    ///
    /// We send to call the final process, the GameLoop
    ///
    GameLoop();

    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

///
/// Function to run FrameBuffer processes
///
HRESULT ClaseOpenGL::FrameBuffer() {

    glGenFramebuffers(1, &m_frameBufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);

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
    return false;
}

///
/// Function to detect window output
///
void ClaseOpenGL::processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

///
/// Receive inputs
///
void CreateInputCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {

    g_CurrentCamera->Input(key);
}

///
/// Receive mouse inputs
///
void MouseButtonCallBack(GLFWwindow*_window, int _button, int _action, int _mods) {

    if (_button == GLFW_MOUSE_BUTTON_LEFT && _action == GLFW_PRESS) {

        g_CurrentCamera->m_ClickPressed = true;
        
        POINT mousePos;

        GetCursorPos(&mousePos);
        g_CurrentCamera->SetOriginalMousePos(mousePos.x, mousePos.y);
    }
    else if (_button == GLFW_MOUSE_BUTTON_LEFT && _action == GLFW_RELEASE) {

        g_CurrentCamera->m_ClickPressed = false;
    }
}

///
/// Function to be able to initialize the values of the window
///
int ClaseOpenGL::WindowGLFW() {

    ///
    /// GLFW window
    ///
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(800, 800, "Ventana de OpenGL", NULL, NULL);
    
    glfwSetKeyCallback(m_window, CreateInputCallBack);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallBack);

    if (m_window == NULL) {

        std::cout << "Falla al crear una ventana en GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    ///
    /// Needed in core profile
    ///
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    InitCameras();
    InitDevice();

    return 0;
}

///
/// Function to start the windowand its processes
///
void ClaseOpenGL::GameLoop(){

    float color1 = 0;
    float color2 = 0;
    float color3 = 0;

    ///
    /// Cycle to have the window runnings
    ///
    while (!glfwWindowShouldClose(m_window)) {

        color1 += 0.02f;
        color2 = cos(color1);
        color3 = sin(color1);

        if (g_CurrentCamera->m_ClickPressed) {

            g_CurrentCamera->MouseRotation();
        }

        glUseProgram(m_programShaderID);

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        mathfu::float4x4 word = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
        };

        mathfu::float4x4 view = m_FPSCamera.GetView();
        view = view.Transpose();
        mathfu::float4x4 projection = m_FPSCamera.GetProjection();
        projection = projection.Transpose();

        ///
        /// word matrix
        ///
        GLuint MatrixID = glGetUniformLocation(m_programShaderID, "WordMatrix");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &word[0]);

        ///
        /// view matrix
        ///
        MatrixID = glGetUniformLocation(m_programShaderID, "ViewMatrix");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0]);

        ///
        /// projection matrix
        ///
        MatrixID = glGetUniformLocation(m_programShaderID, "ProjectionMatrix");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projection[0]);

        for (int i = 0; i < m_SceneManager.m_MeshInScene.size(); i++) {

           
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);

            glVertexAttribPointer
            (
                ///
                /// attribute 0. No particular reason for 0, but must match the layout in the shader.
                ///
                0,

                ///
                /// size
                ///
                3,

                ///
                /// type
                ///
                GL_FLOAT,

                ///
                /// normalized?
                ///
                GL_FALSE,

                ///
                /// stride
                ///
                sizeof(SimpleVertex),

                ///
                /// array buffer offset
                ///
                (void*)0
            );

            glBindBuffer(GL_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);
            glVertexAttribPointer
            (
                ///
                /// attribute 0. No particular reason for 0, but must match the layout in the shader.
                ///
                1,

                ///
                /// size
                ///
                2,

                ///
                /// type
                ///
                GL_FLOAT,

                ///
                /// normalized?
                ///
                GL_FALSE,

                ///
                /// stride
                ///
                sizeof(SimpleVertex),

                ///
                /// array buffer offset
                ///
                (unsigned char*)NULL + (3 * sizeof(float))
            );

            ///
            /// Draw the triangle
            ///
            //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetIndexBuffer()->m_ID);

            ///
            ///1st attribute buffer : vertices
            ///
            GLuint texID = glGetUniformLocation(m_programShaderID, "textureDifuse");

            glUniform1i(texID, m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

            glActiveTexture(GL_TEXTURE0 + m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

            //glBindBuffer(GL_ARRAY_BUFFER, g_manager.m_meshes[i].uvBuffer);
            //glActiveTexture(g_manager.m_meshes[i].m_tex.m_textureID);

            glBindTexture(GL_TEXTURE_2D, m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);
            //glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, g_manager.WindowSize.m_Width, g_

            ///
            ///Draw the triangles
            ///
            glDrawElements
            (
                ///
                ///mode
                ///
                GL_TRIANGLES,

                ///
                ///count
                ///
                m_SceneManager.m_MeshInScene[i]->GetIndexNum(),

                ///
                ///type
                ///
                GL_UNSIGNED_SHORT,
                
                ///
                ///element array buffer offset
                ///
                (void*)0
            );

            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(0);
        }
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

///
/// Function to initialize cameras
///
void ClaseOpenGL::InitCameras()
{
    CameraDescriptor GodCamera;
    GodCamera.s_At = { 0,0,0 };
    GodCamera.s_Eye = { 0,0,-10 };
    GodCamera.s_Up = { 0,1,0 };
    GodCamera.s_Far = 100000;
    GodCamera.s_Near = 0.01;

#ifdef D3D11
    GodCamera.s_FoV = XM_PIDIV4;
#endif

    GodCamera.s_Height = 800;
    GodCamera.s_Widht = 800;

    CameraDescriptor FirstCamera;
    FirstCamera.s_At = { 0,0,0 };
    FirstCamera.s_Eye = { 0,0,20 };
    FirstCamera.s_Up = { 0,1,0 };
    FirstCamera.s_Far = 1000;
    FirstCamera.s_Near = 0.01;

#ifdef D3D11
    FirstCamera.s_FoV = XM_PIDIV4;
#endif

    FirstCamera.s_Height = 800;
    FirstCamera.s_Widht = 800;

    m_FreeCamera.Init(GodCamera);
    m_FPSCamera.Init(FirstCamera);

    g_CurrentCamera = &m_FPSCamera;
}
