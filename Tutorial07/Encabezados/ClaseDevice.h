#pragma once
#include "Defines.h"

///
/// Device Structure
///
struct DeviceDescriptor {

	UINT s_createDeviceFlags;
	UINT s_numFeatureLevels;

	DRIVER_TYPE		g_driverType;
	FEATURE_LEVEL	g_featureLevel;
};

///
/// Class with Device functions
///
class ClaseDevice {

	public:
		
		DeviceDescriptor m_DescDevice;
		
		//
		ClaseDevice() {};
		~ClaseDevice() {};

		//
		void
		Init(DeviceDescriptor _devDesc);
		void
		Update();
		void
		Render();
		void
		Delete();

		void*
		GetDev();

#ifdef D3D11
		ID3D11Device*		g_pd3dDeviceD3D11;
		D3D_DRIVER_TYPE		g_driverTypeD3D11;
		D3D_FEATURE_LEVEL	g_featureLevelD3D11;
#endif // D3D11
};