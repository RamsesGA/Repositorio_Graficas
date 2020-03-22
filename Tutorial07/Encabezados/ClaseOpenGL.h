#pragma once
#include "ClaseRenderTarget.h"
#include "ClaseRenderTargetView.h"
#include "GraphicApi.h"
#include <Windows.h>
#include "Encabezados/ClaseBuffer.h"
#include "SceneManager.h"
#include "Camera.h"
#include "FirstCamera.h"
class ClaseOpenGL {

	public:
		ClaseOpenGL(){};
		~ClaseOpenGL(){};

		//------------------
		//Miembros
		SCENEMANAGER m_SceneManager;
		GraphicApi m_GraphicApi;
		RenderTarget m_RenderTarget;
		ClaseRenderTargetView m_RenderTargetView;
		GLuint m_frameBufferName;
		GLuint m_programShaderID;
		ClaseBuffer m_Buffers;
		GLFWwindow* m_window;
		Camera                      m_FreeCamera;
		FirstCamera                 m_FPSCamera;
		//Si no funciona, recuerdaa mover a window en su función correspondiente 

		//------------------
		//Metodos
		void 
		InitDevice();
		HRESULT 
		FrameBuffer();
		void 
		processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
		int 
		WindowGLFW();
		void 
		GameLoop();
		void 
		InitCameras();
};