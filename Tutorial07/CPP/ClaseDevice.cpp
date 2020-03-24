#include "..\Encabezados\ClaseDevice.h"

///
/// function to initialize the Device variable
///
void ClaseDevice::Init(DeviceDescriptor _devDesc){

	m_DescDevice = _devDesc;
}

void ClaseDevice::Update(){
}

void ClaseDevice::Render(){
}

void ClaseDevice::Delete(){
}

///
/// function to get data from Dev
///
void* ClaseDevice::GetDev(){

#if defined(D3D11)
	return  g_pd3dDeviceD3D11;
#endif
	return nullptr;
}