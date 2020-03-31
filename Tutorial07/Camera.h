#pragma once
#include "Encabezados/Defines.h"
#include "mathfu/hlsl_mappings.h"
#include "Encabezados/ClaseBuffer.h"
#include <xnamath.h>

///
/// Structure of the camera descriptor
///
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

///
/// Father class camera
///
class Camera {

	protected:

		///
		/// Members needed for the chamber
		///
		CameraDescriptor m_Desc;

		///
		/// Members needed for the chamber
		///
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

		///
		/// Members needed for the chamber
		///
		mathfu::float4x4 m_Projection;
		mathfu::float4x4 m_Axis;
		mathfu::float4x4 m_Position;

		///
		/// Members needed for the chamber
		///
#ifdef D3D11
		XMMATRIX m_MProjection;
#endif // D3D11

#ifdef OPENGL
		XMMATRIX m_MProjection;
#endif // OPENGL

		///
		/// Members needed for the chamber
		///
		float angule;
		float maxAngule = 60;

	public:

		///
		/// Members needed for the chamber
		///
		mathfu::float2 OriginalMousePos;

		ClaseBuffer		g_pCBNeverChangesCamera;
		ClaseBuffer		g_pCBChangeOnResizeCamera;
		ClaseBuffer		g_pCBChangesEveryFrameCamera;

		bool m_ClickPressed = false;

		mathfu::float4x4 m_View;

		///
		/// Functions
		///
		int 
		Init(CameraDescriptor _cameraDesc);

		///
		/// Get
		///
		mathfu::float3 GetMUp();
		mathfu::float3 GetMFront();
		mathfu::float3 GetMRight();

		///
		/// Set / Get structures of projection
		///
		void
		SetProjection(mathfu::float4x4 _matProj);
		mathfu::float4x4 
		GetProjection() { return m_Projection; };
		//-----
		void
		SetView(mathfu::float4x4 _view);
		mathfu::float4x4
		GetView() { return m_View; };

		///
		/// Set / Get structures
		///
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
		
		///
		/// General functions for camera operation
		///

		virtual void 
		UpdateVM();
		
		void
		GenerateProjectionMatrix();
		
		virtual void
		CreateView();

		///
		/// Functions for camera movement DIRECTX
		///
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

		///
		/// Functions for camera movement OPENGL
		///
		void
		Input(int _param);

		void
		Move(int _param);

		void
		PitchX(int _param);

		void
		YawZ(int _param);

		void
		RollY(int _param);

		///
		/// Constructor and Destroyer
		///
		Camera();

		~Camera();
};