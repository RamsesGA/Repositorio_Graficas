#include "..\Encabezados\MaterialApi.h"

MaterialApi::MaterialApi(){

#ifdef  D3D11
	m_TexDif  = nullptr;
	m_TexNorm = nullptr;
	m_TexSpec = nullptr;
#endif //  D3D11

}

MaterialApi::~MaterialApi(){


}

void MaterialApi::Shutdown(){

#ifdef D3D11
	if (m_TexDif) {

		m_TexDif->Release();
	}
	//Nuevo
	if (m_TexNorm) {

		m_TexNorm->Release();
	}
	if (m_TexSpec) {

		m_TexSpec->Release();
	}
#endif // D3D11
}