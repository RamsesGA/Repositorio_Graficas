#include "Encabezados/ClaseOpenGL.h"
#include "Encabezados/ClaseShader.h"
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);
}

void ClaseOpenGL::InitDevice(){

    //FrameBuffer();
    m_RenderTarget.InitFrameBuffer();

    //Inicialización de los modelos
    m_SceneManager.Init();

    //Carga de modelos
    m_GraphicApi.ChargeMesh("ugandan-knuckles/source/Knuckles.fbx", m_GraphicApi.m_Model, &m_SceneManager);

    //Load vertex and pixel
    m_programShaderID = ClaseShader::LoadShaders("GBufferVS.glsl", "GBufferPS.glsl");
    glUseProgram(m_programShaderID);

    m_Buffers.OpenGLVAO();
    GameLoop();

    glfwDestroyWindow(m_window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

HRESULT ClaseOpenGL::FrameBuffer() {

    glGenFramebuffers(1, &m_frameBufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);

    // The texture we're going to render to
    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {}
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return false;
}

void ClaseOpenGL::processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int ClaseOpenGL::WindowGLFW() {

    //GLFW window
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

    // Needed in core profile
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    InitDevice();

    return 0;
}

void ClaseOpenGL::GameLoop(){

    float color1 = 0;
    float color2 = 0;
    float color3 = 0;

    while (!glfwWindowShouldClose(m_window)) {

        color1 += 0.02f;
        color2 = cos(color1);
        color3 = sin(color1);

        processInput(m_window);

        glUseProgram(m_programShaderID);

        glClearColor(color2, 255.0f, color3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);


        for (int i = 0; i < m_SceneManager.m_MeshInScene.size(); i++) {

            // 1st attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetVertexBuffer()->m_ID);
            glVertexAttribPointer
            (
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
            );

            // Draw the triangle
            //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_SceneManager.m_MeshInScene[i]->GetIndexBuffer()->m_ID);

            // Draw the triangles
            glDrawElements
            (
                GL_TRIANGLES,       // mode
                6,                  // count
                GL_UNSIGNED_INT,    // type
                (void*)0            // element array buffer offset
            );

            glDisableVertexAttribArray(0);

        }
        

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}