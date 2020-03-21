#pragma once
#include "Defines.h"

struct ShaderDesc {

};

class ClaseShader {

	public:
		ClaseShader() {
#ifdef D3D11
			m_pVSBlobD3D11 = NULL;
#endif // D3D11
			 };

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
		ID3D11InputLayout* LayoutD3D11;
#endif 
		static GLuint 
		LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
};