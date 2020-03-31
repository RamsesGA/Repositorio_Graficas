#pragma once
#include "ClaseRenderTarget.h"
#include "ClaseRenderTargetView.h"
#include "GraphicApi.h"
#include <Windows.h>
#include "Encabezados/ClaseBuffer.h"
#include "SceneManager.h"
#include "Camera.h"
#include "FirstCamera.h"

///
/// Class with all the essential functions for OpenGL
///
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
		FirstCamera             m_FreeCamera;
		FirstCamera             m_FPSCamera;

		///
		/// OPENGL Members
		///
#ifdef OPENGL
		GLuint		m_frameBufferName;
		GLuint		m_programShaderID;
		GLFWwindow* m_window;
		GLuint		m_TextureBillboard;
		GLuint		m_vertexBufferBillBoard;
		GLuint		m_IndexBufferBillBoard;
		GLuint		renderedTexture;
#endif // OPENGL

		
#ifdef OPENGL

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

		///
		/// Methods for the operation of the billboard
		///
		void
		BillBoard();
		void
		UpdateBillBoard();

		///
		/// Methods for the operation of the render on the billboard
		///
		void
		render1();
		
		void
		render2();
#endif // OPENGL
};