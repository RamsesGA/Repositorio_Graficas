#include "..\Encabezados\ClaseDeviceContext.h"

void ClaseDeviceContext::Init(ID3D11DeviceContext* _context){

	g_pImmediateContextD3D11 = _context;
}

void ClaseDeviceContext::Update(){
}

void ClaseDeviceContext::Render(){
}

void ClaseDeviceContext::Delete(){
}
