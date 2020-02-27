#pragma once
/*#include "ClaseDevice.h"
#include "ClaseShader.h"
#include "ClaseViewport.h"
#include "ClaseSwapChain.h"
#include "ClaseSampleState.h"
#include "ClaseDepthStencil.h"
#include "ClaseRenderTarget.h"
#include "ClaseDeviceContext.h"
#include "ClaseRenderTargetView.h"
#include "mathfu/hlsl_mappings.h"
#include "../Camera.h"
#include "../FirstCamera.h"

struct SimpleVertex{

	mathfu::float3 Pos;
	mathfu::float2 Tex;
};
struct CBNeverChanges{

	mathfu::float4x4 mView;
};
struct CBChangeOnResize{

	mathfu::float4x4 mProjection;
};
struct CBChangesEveryFrame{

	mathfu::float4x4 mWorld;
	mathfu::float4 vMeshColor;
};
enum LevelStuff {

	Wall = 1,
	Void = 2,
	m_Pilares = 4,
	NotColideWalls = 5
};

class ClaseManagerInterfaz {

	public:
		//Ramses´s global variables
		MSG m_msg					= { 0 };
		ClaseDevice					m_CDev;
		ClaseDeviceContext			m_CDevCont;
		ClaseSwapChain				m_CSwap;
		ClaseRenderTargetView		m_CRendTarView;
		RenderTarget				m_CRendTar;
		ClaseSampleState			m_CSampleS;
		ClaseDepthStencil			m_CDepthS;
		ClaseViewport				m_CView;
		ClaseShader					m_CShader;

		UINT						m_width;
		UINT						m_height;
		HWND						m_hWnd		= NULL;
		HINSTANCE					m_hInst		= NULL;

		ID3D11Buffer*				m_pVertexBuffer			= NULL;
		ID3D11Buffer*				m_pIndexBuffer			= NULL;
		ID3D11Buffer*				m_pCBNeverChanges		= NULL;
		ID3D11Buffer*				m_pCBChangeOnResize		= NULL;
		ID3D11Buffer*				m_pCBChangesEveryFrame	= NULL;
		ID3D11ShaderResourceView*	m_pTextureRV			= NULL;
		ID3D11PixelShader*			m_pPixelShader			= NULL;

		XMMATRIX                    m_World;
		XMMATRIX                    m_View;
		XMMATRIX                    m_Projection;
		mathfu::float4				m_vMeshColor;

		Camera						m_QuieroDormir_Camara;
		FirstCamera					m_QuieroDormir2_Camara;

		bool						m_ClickPressed			= false;
		int							m_LevelCubes[10][10]	= { 0 };
		int							m_Rows					= 0;
		int							m_Columns				= 0;
		int							m_SwitchCamera			= -1;

		//Metodos
		HRESULT
		Init();

		void
		Update();

		void
		Render();

		void
		Destroy();

		int WINAPI
		wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

		HRESULT
		InitWindow(HINSTANCE hInstance, int nCmdShow);

		HRESULT
		CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

		HRESULT
		CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);

		LRESULT CALLBACK    
		WndProc(HWND, UINT, WPARAM, LPARAM);

		void 
		Resize();

		void 
		InitCameras();
};*/