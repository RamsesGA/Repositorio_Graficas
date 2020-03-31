#pragma once
#include"Encabezados/Defines.h"
#include "mathfu/hlsl_mappings.h"
#include "Camera.h"

///
/// First Person Camera Descriptor
///
struct FirstCameraDescriptor {

	float s_Widht;
	float s_Height;
	float s_Near;
	float s_Far;
	float s_FoV;

	mathfu::float3 s_Eye;
	mathfu::float3 s_At;
	mathfu::float3 s_Up;
};

///
/// Child class of the camera class
///
class FirstCamera :public Camera{

	public:
		
		///
		/// Builder and destroyer
		///
		FirstCamera();
		~FirstCamera();
		
		///
		/// Main methods for the class
		///
		void
		MouseRotation() override;
		
		void
		UpdateVM()override;
		
		void
		CreateView()override;
};