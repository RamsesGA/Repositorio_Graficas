#pragma once
#include"Encabezados/Defines.h"
#include "mathfu/hlsl_mappings.h"
#include "Camera.h"

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

class FirstCamera :public Camera{

	public:
		
		FirstCamera();
		~FirstCamera();
		
		void
		MouseRotation() override;
		
		void
		UpdateVM()override;
		
		void
		CreateView()override;
};