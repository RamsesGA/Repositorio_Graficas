#include "Encabezados/ClaseOpenGL.h"
#include "Encabezados/ClaseShader.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "../Encabezados/stb_image.h"
#include <iostream>
#include <xnamath.h>

///
/// Global variables
///
Camera* g_CurrentCamera;
Camera* g_SecondCamera;
float g_DirLight2[3] = { -1.0f, 0.0f, 0.0f };


///
/// false Current, true Second
///
bool g_WhichCamera = false;

///
/// The callback function receives the new size of the framebuffer when it is resized, which can for example be used to update the OpenGL viewport.
///
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    
#ifdef OPENGL
    glViewport(0, 0, width, height);
#endif // OPENGL
}

#ifdef OPENGL

/// Initialization function
void ClaseOpenGL::InitDevice() {

    //FrameBuffer();
    m_RenderTarget.InitFrameBuffer();

    /// Initialization of the models
    m_SceneManager.Init();

    /// Loading models
    
    //m_GraphicApi.ChargeMesh("ugandan-knuckles/source/Knuckles.fbx", m_GraphicApi.m_Model, &m_SceneManager);
    m_GraphicApi.ChargeMesh("EscenaDelMaestro/Model/Scene/Scene.fbx", m_GraphicApi.m_Model, &m_SceneManager);

    /// Load vertex and pixel shader
    //m_programShaderID = ClaseShader::LoadShaders("GBufferVS.glsl", "GBufferPS.glsl");
    m_programShaderID = ClaseShader::LoadShaders("OpenGLBufferVS.glsl", "OpenGLBufferPS.glsl");
    glUseProgram(m_programShaderID);

    m_Buffers.OpenGLVAO();

    /// We send to call the final process, the GameLoop
    GameLoop();

    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/// Function to run FrameBuffer processes
HRESULT ClaseOpenGL::FrameBuffer() {

    glGenFramebuffers(1, &m_frameBufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);

    ///
    /// The texture we're going to render to
    ///

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

    g_CurrentCamera->inputs(key);
}

///
/// Receive mouse inputs
///
void MouseButtonCallBack(GLFWwindow* _window, int _button, int _action, int _mods) {

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse) {

        ImGui_ImplGlfw_MouseButtonCallback(_window, _button, _action, _mods);
        return;
    }

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

/// Function to be able to initialize the values of the window
int ClaseOpenGL::WindowGLFW() {

    /// GLFW window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(800, 800, "Ventana de OpenGL", NULL, NULL);

    if (m_window == NULL) {

        std::cout << "Falla al crear una ventana en GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    /// Needed in core profile
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    InitCameras();
    InitDevice();

    return 0;
}

/// Function to start the windowand its processes
void ClaseOpenGL::GameLoop() {

    float color1 = 0;
    float color2 = 0;
    float color3 = 0;

    FrameBuffer();
    BillBoard();

    /// ImGui initialization for OpenGL
    /// Setup ImGui binding
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(m_window, true);

    /// Setup style
    ImGui::StyleColorsDark();

    /// Functions to identify an input from the client
    glfwSetKeyCallback(m_window, CreateInputCallBack);
    glfwSetMouseButtonCallback(m_window, MouseButtonCallBack);

    /// Cycle to have the window runnings
    while (!glfwWindowShouldClose(m_window)) {

        color1 += 0.02f;
        color2 = cos(color1);
        color3 = sin(color1);

        UpdateBillBoard();

        /// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        /// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        /// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        /// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        ImGui_ImplGlfwGL3_NewFrame();

        //Generamos una matriz de identidad para mWorld
        mathfu::float4x4 word = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
        };

        //Llenamos os datos de nuestra nueva estructura
        PassData lightPass;

        lightPass.isDeep = true;
        lightPass.m_programShaderID = m_programShaderID;
        lightPass.s_RenderTarget.m_IdRenderTarget = 0; ///En OpenGL RTV es igual a 0
        lightPass.s_ChangeEF.lightDir = { g_DirLight2[0], g_DirLight2[1], g_DirLight2[2], 0 };
        lightPass.s_ChangeEF.World = word;
        lightPass.s_ChangeOR.mProjection = g_CurrentCamera->GetProjection();
        lightPass.s_NeverChanges.mView = g_CurrentCamera->GetView();
        lightPass.s_Mesh = &m_SceneManager.m_MeshInScene;

        m_pass.Pass(lightPass);
        ImGuiGL();

        //render1();
        //render2();

        /// Conditions to know which camera is being used
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    /// Cleanup ImGui
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
}

/// Function to initialize cameras
void ClaseOpenGL::InitCameras()
{
    CameraDescriptor FirstCamera;
    FirstCamera.s_At = { 0,0,0 };
    FirstCamera.s_Eye = { 0,0,20 };
    FirstCamera.s_Up = { 0,1,0 };
    FirstCamera.s_Far = 1000;
    FirstCamera.s_Near = 0.01;

#ifdef D3D11
    FirstCamera.s_FoV = XM_PIDIV4;
#endif

#ifdef OPENGL
    FirstCamera.s_FoV = XM_PIDIV4;
#endif // OPENGL

    FirstCamera.s_Height = 800;
    FirstCamera.s_Widht = 800;

    m_FreeCamera.Init(FirstCamera);
    m_FPSCamera.Init(FirstCamera);

    g_CurrentCamera = &m_FPSCamera;
    g_SecondCamera = &m_FreeCamera;
}

/// Function to initialize Billboard
void ClaseOpenGL::BillBoard() {

    glGenTextures(1, &m_TextureBillboard);
    glBindTexture(GL_TEXTURE_2D, m_TextureBillboard);

    ///
    /// set the texture wrapping/filtering options (on the currently bound texture object)
    ///
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ///
    /// load and generate the texture
    ///
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Billboard.jpg", &width, &height, &nrChannels, 0);

    ///
    /// Condition to check the model load
    ///
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    ///
    /// Steps to generate a billboard
    ///
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    ///
    /// We generate vectors to obtain the camera position
    ///
    mathfu::float3 upWordSpace = g_CurrentCamera->GetMUp();
    mathfu::float3 RoghtWordSpace = g_CurrentCamera->GetMRight();
    mathfu::float3 pos;
    SimpleVertex billboard[4];

    ///
    /// We indicate the position of each point, including UV
    ///
    billboard[0].msPos = { -0.01f, -0.01f, 0.0f };
    pos = billboard[0].msPos;
    billboard[0].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[0].texcoord = { -1,1 };

    billboard[1].msPos = { 0.01f, -0.01f, 0.0f };
    pos = billboard[1].msPos;
    billboard[1].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[1].texcoord = { 1, 1 };

    billboard[2].msPos = { -0.01f, 0.01f, 0.0f };
    pos = billboard[2].msPos;
    billboard[2].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[2].texcoord = { -1, -1 };

    billboard[3].msPos = { 0.01f, 0.01f, 0.0f };
    pos = billboard[3].msPos;
    billboard[3].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[3].texcoord = { 1, -1 };

    ///
    /// Generate 1 buffer, put the resulting identifier in vertexbuffer
    ///
    glGenBuffers(1, &m_vertexBufferBillBoard);

    ///
    /// The following commands will talk about our 'vertexbuffer' buffer
    ///
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);

    ///
    /// Give our vertices to OpenGL.
    ///
    glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * 4, &billboard, GL_STATIC_DRAW);

    WORD indexBuffer[] = { 0,1,2,1,3,2 };
    //GLuint elementbuffer;// Index Buffer
    glGenBuffers(1, &m_IndexBufferBillBoard);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferBillBoard);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(WORD), &indexBuffer[0], GL_STATIC_DRAW);
}

/// Function to initialize Billboard
void ClaseOpenGL::UpdateBillBoard() {

    /// We generate vectors to obtain the camera position
    mathfu::float3 upWordSpace = g_CurrentCamera->GetMUp();
    mathfu::float3 RoghtWordSpace = g_CurrentCamera->GetMRight();
    mathfu::float3 pos;
    SimpleVertex billboard[4];

    /// We indicate the position of each point, including UV
    billboard[0].msPos = { -0.5f, -0.5f, 0.0f };
    pos = billboard[0].msPos;
    billboard[0].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[0].texcoord = { 0,0 };

    billboard[1].msPos = { 0.5f, -0.5f, 0.0f };
    pos = billboard[1].msPos;
    billboard[1].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[1].texcoord = { 1, 0 };

    billboard[2].msPos = { -0.5f, 0.5f, 0.0f };
    pos = billboard[2].msPos;
    billboard[2].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[2].texcoord = { 0, 1 };

    billboard[3].msPos = { 0.5f, 0.5f, 0.0f };
    pos = billboard[3].msPos;
    billboard[3].msPos = RoghtWordSpace * (pos.x * 10)
        + upWordSpace * (pos.y * 10);
    billboard[3].texcoord = { 1, 1 };

    /// The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);

    /// Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * 4, &billboard, GL_STATIC_DRAW);
}


void ClaseOpenGL::ImGuiGL() {

    //EN EL PASS GENERAR UNA FUNCIÓN PARA EL IMGUI
    ImGui::Text("Cambio de Camara");

    /// Buttons return true when clicked (NB: most widgets return true when edited/activated)
    if (ImGui::Button("Button")) {

        //Current to Second
        if (g_WhichCamera == false) {

            g_WhichCamera = true;
        }
        //Second to Current
        else if (g_WhichCamera == true) {

            g_WhichCamera = false;
        }

        /// Camera switch button
        Camera* Temporal = g_SecondCamera;
        g_SecondCamera = g_CurrentCamera;
        g_CurrentCamera = Temporal;
    }

    ImGui::SameLine();

    /// Conditions to know which camera is being used
    if (g_WhichCamera == false) {

        ImGui::Text("Primera Camara");
    }
    else if (g_WhichCamera == true) {

        ImGui::Text("Segunda Camara");
    }

    ImGui::Text("Cambio de Luz");
    ImGui::SliderFloat3("float", g_DirLight2, -1, 1);

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

/// Function to be able to render and load the generation of texture of the first camera
void ClaseOpenGL::render1() {

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);
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

    mathfu::float4x4 view = g_SecondCamera->GetView();
    view = view.Transpose();
    mathfu::float4x4 projection = g_SecondCamera->GetProjection();
    projection = projection.Transpose();

    /// word matrix
    GLuint MatrixID = glGetUniformLocation(m_programShaderID, "WordMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &word[0]);
    
    /// view matrix
    MatrixID = glGetUniformLocation(m_programShaderID, "ViewMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0]);

    /// projection matrix
    MatrixID = glGetUniformLocation(m_programShaderID, "ProjectionMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projection[0]);

    //Dirección de luz
    GLuint idLight = glGetUniformLocation(m_programShaderID, "LightDir");
    float DirLight[4] = { g_DirLight2[0], g_DirLight2[1], g_DirLight2[2], 0 };
    glUniform4fv(idLight, 1, &DirLight[0]);

    for (int i = 0; i < m_SceneManager.m_MeshInScene.size(); i++) {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);

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

        glBindBuffer(GL_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetIndexBuffer()->m_ID);

        ///1st attribute buffer : vertices
        GLuint texID = glGetUniformLocation(m_programShaderID, "textureDifuse");

        glUniform1i(texID, m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        glActiveTexture(GL_TEXTURE0 + m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        glBindTexture(GL_TEXTURE_2D, m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        ///Draw the triangles
        glDrawElements
        (
            ///mode
            GL_TRIANGLES,
            ///count
            m_SceneManager.m_MeshInScene[i]->GetIndexNum(),
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

/// Function to be able to render and load the generation of texture of the second camera
void ClaseOpenGL::render2()
{

    if (g_CurrentCamera->m_ClickPressed) {

        g_CurrentCamera->MouseRotation();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    mathfu::float4x4 view = g_CurrentCamera->GetView();
    view = view.Transpose();
    mathfu::float4x4 projection = g_CurrentCamera->GetProjection();
    projection = projection.Transpose();

    /// word matrix
    GLuint MatrixID = glGetUniformLocation(m_programShaderID, "WordMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &word[0]);
    
    /// view matrix
    MatrixID = glGetUniformLocation(m_programShaderID, "ViewMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &view[0]);

    /// projection matrix
    MatrixID = glGetUniformLocation(m_programShaderID, "ProjectionMatrix");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &projection[0]);

    //Dirección de luz
    GLuint idLight = glGetUniformLocation(m_programShaderID, "LightDir");
    float DirLight[4] = { g_DirLight2[0], g_DirLight2[1], g_DirLight2[2], 0 };
    glUniform4fv(idLight, 1, &DirLight[0]);

    for (int i = 0; i < m_SceneManager.m_MeshInScene.size(); i++) {
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);

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

        glBindBuffer(GL_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);
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
            /// array buffer offset
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetIndexBuffer()->m_ID);

        ///1st attribute buffer : vertices
        GLuint texID = glGetUniformLocation(m_programShaderID, "textureDifuse");

        glUniform1i(texID, m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        glActiveTexture(GL_TEXTURE0 + m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        glBindTexture(GL_TEXTURE_2D, m_SceneManager.m_MeshInScene[i]->m_Materials->m_TextureId);

        ///Draw the triangles
        glDrawElements
        (
            ///mode
            GL_TRIANGLES,

            ///count
            m_SceneManager.m_MeshInScene[i]->GetIndexNum(),

            ///type
            GL_UNSIGNED_SHORT,

            ///element array buffer offset
            (void*)0
            );

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }

    {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);

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

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferBillBoard);
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
            /// array buffer offset
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
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferBillBoard);

        ///1st attribute buffer : vertices
        GLuint texID = glGetUniformLocation(m_programShaderID, "textureDifuse");

        glUniform1i(texID, renderedTexture);

        glActiveTexture(GL_TEXTURE0 + renderedTexture);

        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        
        ///Draw the triangles
        glDrawElements
        (
            ///mode
            GL_TRIANGLES,
            ///count
            6,
            ///type
            GL_UNSIGNED_SHORT,
            ///element array buffer offset
            (void*)0
            );

        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
    }

    /// Render ImGui
    /// 1. Show a simple window.
    // Display some text (you can use a format string too)
    
}
#endif // OPENGL