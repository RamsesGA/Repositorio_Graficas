#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

struct ShaderDesc {

};

class ClaseShader {

	public:
		ClaseShader() { m_pVSBlobD3D11 = NULL; };
		~ClaseShader() {};

		ShaderDesc m_ShaderD;

		void
		Init();

		void
		Update();

		void
		Render();

		void
		Destroy();

#ifdef D3D11
		ID3D11VertexShader* g_pVertexShaderD3D11;
		ID3DBlob* m_pVSBlobD3D11;
#endif // D3D11
};