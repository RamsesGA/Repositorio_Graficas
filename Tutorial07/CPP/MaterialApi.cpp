#include "..\Encabezados\MaterialApi.h"

MaterialApi::MaterialApi(){


}

MaterialApi::~MaterialApi(){


}

void MaterialApi::Shutdown(){

	if (m_TexDif){

		m_TexDif->Release();
	}
}