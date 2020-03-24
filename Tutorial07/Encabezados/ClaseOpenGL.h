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

		///
		/// General Members
		///
		SCENEMANAGER			m_SceneManager;
		GraphicApi				m_GraphicApi;
		RenderTarget			m_RenderTarget;
		ClaseRenderTargetView	m_RenderTargetView;
		ClaseBuffer				m_Buffers;
		Camera                  m_FreeCamera;
		FirstCamera             m_FPSCamera;

		///
		/// OPENGL Members
		///
		GLuint		m_frameBufferName;
		GLuint		m_programShaderID;
		GLFWwindow* m_window;

		///
		/// OPENGL Methods
		///
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