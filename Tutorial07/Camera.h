#pragma once
#include "mathfu/hlsl_mappings.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

struct CameraDescriptor {

	float s_Widht;
	float s_Height;
	float s_Near;
	float s_Far;
	float s_FoV;
	mathfu::float3 s_Eye;
	mathfu::float3 s_At;
	mathfu::float3 s_Up;
};

class Camera {

	protected:

		//Variables-----
		CameraDescriptor m_Desc;

		//-----
		mathfu::float2 OriginalMousePos;

		//-----
		mathfu::float3 m_Up;
		mathfu::float3 m_Front;
		mathfu::float3 m_locketFront;
		mathfu::float3 m_onlyXnY;
		mathfu::float3 m_locketonlyXnY;
		mathfu::float3 m_Right;
		
		mathfu::float3 originalEye;
		mathfu::float3 OriginalUp;
		mathfu::float3 OriginalAt;
		mathfu::float3 m_UpLimit;

		//-----
		mathfu::float4x4 m_View;
		mathfu::float4x4 m_Projection;
		mathfu::float4x4 m_Axis;
		mathfu::float4x4 m_Position;

		//-----
		XMMATRIX m_MProjection;
		float angule;
		float maxAngule = 60;

	public:

		//Functions-----
		int 
		Init(CameraDescriptor _cameraDesc);

		//Get
		mathfu::float3 GetMUp();
		mathfu::float3 GetMFront();
		mathfu::float3 GetMRight();

		//-----
		void
		SetProjection(mathfu::float4x4 _matProj);
		mathfu::float4x4 
		GetProjection() { return m_Projection; };
		//-----
		void
		SetView(mathfu::float4x4 _view);
		mathfu::float4x4
		GetView() { return m_View; };

		//Set/Get structure
		void
		SetWidht(float _widht);
		float
		GetWidht();

		//-----
		void
		SetHeight(float _height);
		float
		GetHeight();

		//-----
		void
		SetNear(float _near);
		float 
		GetNear();

		//-----
		void
		SetFar(float _far);
		float
		GetFar();

		//-----
		void
		SetFoV(float _fOv);
		float
		GetFoV();

		//-----
		void 
		SetEye(mathfu::float3 _vec3);
		mathfu::float3 GetEye();

		//-----
		void 
		SetAt(mathfu::float3 _vec3);
		mathfu::float3 GetAt();

		//-----
		void 
		SetUp(mathfu::float3 _vec3);
		mathfu::float3 GetUp();
		
		//-----
		virtual void 
		UpdateVM();
		
		void
		GenerateProjectionMatrix();
		
		virtual void
		CreateView();
		
		void 
		Move(WPARAM _param);
		
		void
		PitchX(WPARAM _param);
		
		void
		YawZ(WPARAM _param);
		
		void
		RollY(WPARAM _param);
		
		virtual void
		MouseRotation();
		
		void
		SetOriginalMousePos(float _x, float _y) { OriginalMousePos = { _x, _y }; };
		
		void
		inputs(WPARAM _param);

		//Constructor and Destructor-----
		Camera();
		~Camera();
};