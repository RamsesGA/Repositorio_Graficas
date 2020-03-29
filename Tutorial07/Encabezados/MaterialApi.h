#pragma once
#include <string>
#include "Defines.h"

///
/// Class with the items necessary to load the model
///
class MaterialApi
{
	public:
		MaterialApi();
		~MaterialApi();
	
		void 
		Shutdown();

		//Variables
		std::string					m_NormalName;
		bool						m_HasNormals;
		bool						m_HasDifuse;
		std::string					m_Diroftextures;
		std::string					m_DifuseName;

#ifdef OPENGL
		GLuint m_TextureId;
#endif // OPENGL


#ifdef D3D11
		ID3D11ShaderResourceView* m_TexDif = NULL;
		ID3D11ShaderResourceView* m_TexNorm = NULL;
#endif // D3D11
};