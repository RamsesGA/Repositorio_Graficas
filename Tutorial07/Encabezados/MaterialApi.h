#pragma once
#include <string>
#include "Defines.h"

///
/// Class with the items necessary to load the model
///
class MaterialApi
{
	public:

		///
		/// Main methods for the class
		///
		MaterialApi();
		~MaterialApi();
	
		void 
		Shutdown();

		///
		/// Members
		///
		std::string					m_NormalName;
		bool						m_HasNormals;
		bool						m_HasDifuse;
		std::string					m_Diroftextures;
		std::string					m_DifuseName;

		///
		/// Members OPENGL
		///
#ifdef OPENGL
		GLuint m_TextureId;
#endif // OPENGL

		///
		/// Members DIRECTX
		///
#ifdef D3D11
		ID3D11ShaderResourceView* m_TexDif;
		ID3D11ShaderResourceView* m_TexNorm;
		ID3D11ShaderResourceView* m_TexSpec;
#endif // D3D11
};