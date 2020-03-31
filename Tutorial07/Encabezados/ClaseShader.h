#pragma once
#include "Defines.h"

struct ShaderDesc {

};

///
/// Shader Abstraction Class
///
class ClaseShader {

	public:

		///
		/// Builder and destroyer
		///
		ClaseShader() {
#ifdef D3D11
			m_pVSBlobD3D11 = NULL;
#endif // D3D11
			 };

		~ClaseShader() {};

		///
		/// Members
		///
		ShaderDesc m_ShaderD;

		///
		/// Main methods for class operation
		///
		void
		Init();

		void
		Update();
		
		void
		Render();

		void
		Destroy();


		///
		/// DIRECTX members
		///
#ifdef D3D11
		ID3D11VertexShader* g_pVertexShaderD3D11;
		ID3DBlob* m_pVSBlobD3D11;
		ID3D11InputLayout* LayoutD3D11;
#endif 

		///
		/// OPENGL methods
		///
#ifdef OPENGL
		static GLuint
		LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
#endif // OPENGL
};