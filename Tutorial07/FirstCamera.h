#pragma once
#include "mathfu/hlsl_mappings.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

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

class FirstCamera{

	private:
	
		//Variables-----
		FirstCameraDescriptor m_FirstDesc;

		//-----
		mathfu::float2 OriginalMousePos;
	
		//-----
		mathfu::float3 m_Up;
		mathfu::float3 m_Front;
		mathfu::float3 m_Right;
	
		//-----
		mathfu::float4x4 m_View;
		mathfu::float4x4 m_Projection;
		mathfu::float4x4 m_Axis;
		mathfu::float4x4 m_Position;
	
		//-----
		XMMATRIX m_MProjection;
	
	public:
	
		//Functions-----
		int
			FInit(FirstCameraDescriptor _FirstCameraDesc);
	
		//Get
		mathfu::float3 
		FGetMUp() { return m_Up; };
		mathfu::float3 
		FGetMFront() { return m_Front; };
		mathfu::float3
		FGetMRight() { return m_Right; };
	
		//-----
		void
		FSetProjection(mathfu::float4x4 _matProj);
		mathfu::float4x4
		FGetProjection() { return m_Projection; };
		//-----
		void
		FSetView(mathfu::float4x4 _view);
		mathfu::float4x4
		FGetView() { return m_View; };
	
		//Set/Get structure
		void
		FSetWidht(float _widht);
		float
		FGetWidht() { return m_FirstDesc.s_Widht; };
	
		//-----
		void
		FSetHeight(float _height);
		float
		FGetHeight() { return m_FirstDesc.s_Height; };
	
		//-----
		void
		FSetNear(float _near);
		float
		FGetNear() { return m_FirstDesc.s_Near; };
	
		//-----
		void
		FSetFar(float _far);
		float
		FGetFar() { return m_FirstDesc.s_Far; };
	
		//-----
		void
		FSetFoV(float _fOv);
		float
		FGetFoV() { return m_FirstDesc.s_FoV; };
	
		//-----
		void
		FSetEye(mathfu::float3 _vec3);
		mathfu::float3
		FGetEye() { return m_FirstDesc.s_Eye; };
	
		//-----
		void
		FSetAt(mathfu::float3 _vec3);
		mathfu::float3
		FGetAt() { return m_FirstDesc.s_At; };
	
		//-----
		void
		FSetUp(mathfu::float3 _vec3);
		mathfu::float3 
		FGetUp() { return m_FirstDesc.s_Up; };
	
		//-----
		void
		FUpdateVM();
		void
		FGenerateProjectionMatrix();
		void
		FCreateView();
		void
		FMove(WPARAM _param);
		void
		FPitchX(WPARAM _param);
		void
		FYawZ(WPARAM _param);
		void
		FRollY(WPARAM _param);
		void
		FMouseRotation();
		void
		FSetOriginalMousePos(float _x, float _y) { OriginalMousePos = { _x, _y }; };
	
		/*void
		Finputs(WPARAM _param);*/
	
		//Constructor and Destructor-----
		FirstCamera();
		~FirstCamera() {};
};

