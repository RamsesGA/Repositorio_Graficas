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
#include "Encabezados/ClaseTexture2D.h"
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

	D3D11_RASTERIZER_DESC s_rasterizer;
	ClaseDeviceContext* s_deviceContext;
	ClaseDevice* s_device;
	ClaseViewport* s_viewport;
	Texture2Desc s_texture2Desc;
	unsigned int s_renderTargetViewAccountant;
};

class CPass
{
	public:
		//
		//Miembros;
		//
		ClaseBuffer* m_bufferConstantBuffer;
		std::vector<ClaseRenderTargetView*>* m_vectorRTV;
	
		//
		//Métodos
		//
		CPass() : m_deviceContext(nullptr), m_ViewPort(nullptr){};
		CPass() = default;
		~CPass() = default;
	
		//DirectX
	#ifdef D3D11
		//
		//Miembros
		//
	
		//Guardamos los resultados de render targets de otros pases que se llegen a necesitar
		std::vector<ID3D11ShaderResourceView*> m_vectorShaderResorceView;
		//Guardamos las texturas generadas
		std::vector<ClaseTextura2D*> m_texture2D;
		//Resultado de un shader resorce view
		std::vector<ID3D11RenderTargetView*> m_vectorRendTargView;
		ID3D11RasterizerState* m_rasterizerState;
		ClaseDeviceContext* m_deviceContext;
		ClaseShader* m_InputLayout;
		ClaseShader* m_VertexShader;
		ClaseShader* m_PixelShader;
		ClaseViewport* m_ViewPort;
	
		//
		//Métodos
		//
	
		//Función para poder agregarlos al vector de nuestro shader resource
		void SetShaderResource(ClaseDevice* _device, ClaseTextura2D* _texture2d);
	
		//Función para mandar a llamar SetRT y SetShader
		void SetPass(ClaseDepthStencil* _depthStencilView);
	
		//Función para decirle a la tarjeta que dibujar
		void Draw(SCENEMANAGER* _sceneManager, UINT _sizeSimpleVertex);
			
		//Función para limpiar la pantalla
		void ClearWindow(float _clearColor[4], ClaseDepthStencil* _depthStencilView);
	
		//Le decimos a la tarjeta que shader usar
		void SetShader();
	
		//Le decimos al shader el o las diferentes RT que va a dibujar
		void SetRenderTarget(ClaseDepthStencil* _depthStencilView);
	
		void Pass(PassData& _sPassData);
	
		void Render(ClaseRenderTargetView& _renderTargView, ClaseDepthStencil& _depthStencil, ClaseDeviceContext* _devContext, SCENEMANAGER& _sceneManager, Camera* _camera, ClaseBuffer* _light);
	
		void Init(PassDX _struct);
	
	#else
		//OpenGL
		void Pass(PassData& _sPassData);
	#endif
};
