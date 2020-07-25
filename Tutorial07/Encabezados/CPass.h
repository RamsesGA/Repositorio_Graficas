#pragma once
#include <vector>
#include "Encabezados/ClaseRenderTarget.h"
#include "Encabezados/ClaseSwapChain.h"
#include "Encabezados/SceneManager.h"
#include "Encabezados/ClaseSampleState.h"
#include "Encabezados/ClaseShader.h"
#include "Encabezados/ClaseVertexBuffer.h"
#include "Encabezados/ClaseDepthStencil.h"
#include "Encabezados/ClaseDeviceContext.h"
#include "Encabezados/ClaseViewport.h"
#include "Encabezados/ClaseRenderTargetView.h"
#include "Camera.h"

struct PassData {

	//Solo ponerle true al primero
	bool isDeep = false;

#ifdef OPENGL
	GLuint							m_programShaderID;
#endif

	//Se vuelven punteros para que no estén copiando la información
	std::vector <MESH*>* s_Mesh;
	ClaseDeviceContext				s_cDevCont;
	RenderTarget					s_RenderTarget;

	//Buffers
	CBChangesEveryFrame				s_ChangeEF;
	CBNeverChanges					s_NeverChanges;
	CBChangeOnResize				s_ChangeOR;
	Lights							s_Lights;
};

struct PassDX {

	ClaseShader* s_InputLayout;

	ClaseShader* s_VertexShader;

	ClaseShader* s_PixelShader;

	ClaseViewport* s_ViewPort;

	ClaseBuffer* s_boneBuffer;
};

class CPass
{

	public:
		CPass() {};
		~CPass() {};
		
		ClaseBuffer* m_bufferConstantBuffer;

#ifdef D3D11
		//DirectX
		void Pass(PassData& _sPassData);

		PassDX m_PassDX;

		void Render(ClaseRenderTargetView& _renderTargView, ClaseDepthStencil& _depthStencil, ClaseDeviceContext* _devContext, SCENEMANAGER& _sceneManager, Camera* _camera, ClaseBuffer* _light);

		int Init(PassDX& _struct);
#else
		//OpenGL
		void Pass(PassData& _sPassData);
#endif
};
