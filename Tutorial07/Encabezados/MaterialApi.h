#pragma once
#include <string>
#include "Defines.h"

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

		GLuint m_TextureId;

#ifdef D3D11
		ID3D11ShaderResourceView* m_TexDif = NULL;
		ID3D11ShaderResourceView* m_TexNorm = NULL;
#endif // D3D11

		
};