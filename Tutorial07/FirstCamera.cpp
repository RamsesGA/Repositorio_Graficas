#include "FirstCamera.h"

//-----
FirstCamera::FirstCamera(){

	m_FirstDesc.s_Widht =	0.0f;
	m_FirstDesc.s_Height =	0.0f;
	m_FirstDesc.s_Near =	0.0f;
	m_FirstDesc.s_Far =		0.0f;
	m_FirstDesc.s_FoV =		0.0f;

	m_Front =	mathfu::float3(0.0f, 0.0f, 0.0f);
	m_Up =		mathfu::float3(0.0f, 0.0f, 0.0f);
	m_Right =	mathfu::float3(0.0f, 0.0f, 0.0f);
	m_FirstDesc.s_Eye = mathfu::float3(0.0f, 0.0f, 0.0f);
	m_FirstDesc.s_At =	mathfu::float3(0.0f, 0.0f, 0.0f);
}

//-----
int FirstCamera::FInit(FirstCameraDescriptor _FirstCameraDesc){

	m_FirstDesc = _FirstCameraDesc;

	FSetWidht(m_FirstDesc.s_Widht);
	FSetHeight(m_FirstDesc.s_Height);
	FSetNear(m_FirstDesc.s_Near);
	FSetFar(m_FirstDesc.s_Far);
	FSetFoV(m_FirstDesc.s_FoV);
	FSetEye(m_FirstDesc.s_Eye);
	FSetAt(m_FirstDesc.s_At);
	FSetUp(m_FirstDesc.s_Up);

	FCreateView();
	FGenerateProjectionMatrix();
	FUpdateVM();

	return 0;
}

//-----
void FirstCamera::FSetProjection(mathfu::float4x4 _matProj){

	m_Projection = _matProj;
}

//-----
void FirstCamera::FSetView(mathfu::float4x4 _view){

	m_View = _view;
}

//-----
void FirstCamera::FSetWidht(float _widht){

	m_FirstDesc.s_Widht = _widht;
}

//-----
void FirstCamera::FSetHeight(float _height){

	m_FirstDesc.s_Height = _height;
}

//-----
void FirstCamera::FSetNear(float _near){

	m_FirstDesc.s_Near = _near;
}

//-----
void FirstCamera::FSetFar(float _far){

	m_FirstDesc.s_Far = _far;
}

//-----
void FirstCamera::FSetFoV(float _fOv){

	m_FirstDesc.s_FoV = _fOv;
}

//-----
void FirstCamera::FSetEye(mathfu::float3 _vec3){

	m_FirstDesc.s_Eye = _vec3;
}

//-----
void FirstCamera::FSetAt(mathfu::float3 _vec3){

	m_FirstDesc.s_At = _vec3;
}

//-----
void FirstCamera::FSetUp(mathfu::float3 _vec3){

	m_FirstDesc.s_Up = _vec3;
}

//-----
void FirstCamera::FUpdateVM(){

	m_Right = { m_View.data_[0].x, m_View.data_[0].y, m_View.data_[0].z };
	m_Up = { m_View.data_[1].x, m_View.data_[1].y, m_View.data_[1].z };
	m_Front = { m_View.data_[2].x, m_View.data_[2].y, m_View.data_[2].z };
}

//-----
void FirstCamera::FGenerateProjectionMatrix(){

	m_MProjection = XMMatrixPerspectiveFovLH
	(
		m_FirstDesc.s_FoV,
		m_FirstDesc.s_Widht / m_FirstDesc.s_Height,
		m_FirstDesc.s_Near,
		m_FirstDesc.s_Far
	);

	m_Projection =
	{
		m_MProjection._11, m_MProjection._12, m_MProjection._13, m_MProjection._14,
		m_MProjection._21, m_MProjection._22, m_MProjection._23, m_MProjection._24,
		m_MProjection._31, m_MProjection._32, m_MProjection._33, m_MProjection._34,
		m_MProjection._41, m_MProjection._42, m_MProjection._43, m_MProjection._44
	};

	m_Projection = m_Projection.Transpose();
}

//-----
void FirstCamera::FCreateView(){

	//Usamos left hand
	m_Front = m_FirstDesc.s_At - m_FirstDesc.s_Eye;
	m_Front.Normalize();

	m_Right = mathfu::cross(m_FirstDesc.s_Up, m_Front);
	m_Right.Normalize();

	m_Up = mathfu::cross(m_Front, m_Right);

	m_Axis =
	{
		m_Right.x,	m_Right.y,	m_Right.z,	0,
		m_Up.x,		m_Up.y,		m_Up.z,		0,
		m_Front.x,	m_Front.y,	m_Front.z,	0,
		0,			0,			0,			1
	};

	m_Position =
	{
		1,	0,	0, -m_FirstDesc.s_Eye.x,
		0,	1,	0, -m_FirstDesc.s_Eye.y,
		0,	0,	1, -m_FirstDesc.s_Eye.z,
		0,	0,	0,	1
	};

	m_Position *= m_Axis;
	m_View = m_Position; //Ya está la matriz view
}

//-----
void FirstCamera::FMove(WPARAM _param){

	if (_param == 'w' || _param == 'W') {

		m_FirstDesc.s_Eye += m_Front;
	}
	else if (_param == 's' || _param == 'S') {

		m_FirstDesc.s_Eye -= m_Front;
	}
	else if (_param == 'a' || _param == 'A') {

		m_FirstDesc.s_Eye -= m_Right;
	}
	else if (_param == 'd' || _param == 'D') {

		m_FirstDesc.s_Eye += m_Right;
	}

	//-----
	m_Axis =
	{
		m_Right.x,	m_Right.y,	m_Right.z,	0,
		m_Up.x,		m_Up.y,		m_Up.z,		0,
		m_Front.x,	m_Front.y,	m_Front.z,	0,
		0,			0,			0,			1
	};

	m_Position =
	{
		1,	0,	0,	-m_FirstDesc.s_Eye.x,
		0,	1,	0,	-m_FirstDesc.s_Eye.y,
		0,	0,	1,	-m_FirstDesc.s_Eye.z,
		0,	0,	0,	1
	};

	m_Position *= m_Axis;
	m_View = m_Position; //Ya está la matriz view
	FUpdateVM();
}

//-----
void FirstCamera::FPitchX(WPARAM _param){

	mathfu::float4x4 rot;
	float speedrot = 0.10f;

	if (_param == VK_UP) {

		rot =
		{
			1,	0,				0,					0,
			0,	cosf(speedrot),	-sinf(speedrot),	0,
			0,	sinf(speedrot),	cosf(speedrot),		0,
			0,	0,				0,					1
		};
	}
	else if (_param == VK_DOWN) {

		rot =
		{
			1,	0,					0,					0,
			0,	cosf(-speedrot),	-sinf(-speedrot),	0,
			0,	sinf(-speedrot),	cosf(-speedrot),	0,
			0,	0,					0,					1
		};
	}
	m_View *= rot;
	FUpdateVM();
}

//-----
void FirstCamera::FYawZ(WPARAM _param){

	mathfu::float4x4 rot;
	float speedrot = 0.10f;

	if (_param == 'z' || _param == 'Z') {

		rot =
		{
			cosf(speedrot),		0,	sinf(speedrot),	0,
			0,					1,	0,				0,
			-sinf(speedrot),	0,	cosf(speedrot),	0,
			0,					0,	0,				1
		};
	}
	else if (_param == 'c' || _param == 'C') {

		rot =
		{
			cosf(-speedrot),		0,	sinf(-speedrot),	0,
			0,						1,	0,					0,
			-sinf(-speedrot),		0,	cosf(-speedrot),	0,
			0,						0,	0,					1
		};
	}
	m_View *= rot;
	FUpdateVM();
}

//-----
void FirstCamera::FRollY(WPARAM _param){

	mathfu::float4x4 rot;
	float speedrot = 0.10f;

	if (_param == VK_RIGHT) {

		rot =
		{
			cosf(speedrot),	-sinf(speedrot),	0,	0,
			sinf(speedrot),	cosf(speedrot),		0,	0,
			0,				0,					1,	0,
			0,				0,					0,	1
		};
	}
	else if (_param == VK_LEFT) {

		rot =
		{
			cosf(-speedrot),	-sinf(-speedrot),		0,		0,
			sinf(-speedrot),	cosf(-speedrot),		0,		0,
			0,				0,							1,		0,
			0,				0,							0,		1
		};
	}
	m_View *= rot;
	FUpdateVM();
}

//-----
void FirstCamera::FMouseRotation(){

	mathfu::float2 FirstPos;
	mathfu::float2 SecondPos;
	float SpeedRot = 0.00030f;

	mathfu::float4x4 Yaw =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	mathfu::float4x4 Pitch =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	POINT Temp;
	GetCursorPos(&Temp);

	FirstPos.x = Temp.x;
	SecondPos.y = Temp.y;

	if (FirstPos.x < OriginalMousePos.x && m_limit != 0.0012f) {
		
		m_limit = m_limit + SpeedRot;

		Yaw =
		{
			cosf(SpeedRot),		0,	sinf(SpeedRot),	0,
			0,					1,	0,				0,
			-sinf(SpeedRot),	0,	cosf(SpeedRot),	0,
			0,					0,	0,				1
		};
		m_View *= Yaw;
		FUpdateVM();
	}
	if (FirstPos.x > OriginalMousePos.x && m_limit != -0.0012f) {

		m_limit = m_limit - SpeedRot;

		Yaw =
		{
			cosf(-SpeedRot),		0,	sinf(-SpeedRot),	0,
			0,						1,	0,					0,
			-sinf(-SpeedRot),		0,	cosf(-SpeedRot),	0,
			0,						0,	0,					1
		};
		m_View *= Yaw;
		FUpdateVM();
	}

	if (SecondPos.y < OriginalMousePos.y && m_limit != 0.0012f) {

		m_limit = m_limit + SpeedRot;

		Pitch =
		{
			1,	0,				0,					0,
			0,	cosf(SpeedRot),	-sinf(SpeedRot),	0,
			0,	sinf(SpeedRot),	cosf(SpeedRot),		0,
			0,	0,				0,					1
		};
		m_View *= Pitch;
		FUpdateVM();
	}
	if (SecondPos.y > OriginalMousePos.y&& m_limit != -0.0012f) {

		m_limit = m_limit - SpeedRot;

		Pitch =
		{
			1,	0,					0,					0,
			0,	cosf(-SpeedRot),	-sinf(-SpeedRot),	0,
			0,	sinf(-SpeedRot),	cosf(-SpeedRot),	0,
			0,	0,					0,					1
		};
		m_View *= Pitch;
		FUpdateVM();
	}

	SetCursorPos(OriginalMousePos.x, OriginalMousePos.y);
	
}

/*
//-----
void FirstCamera::Finputs(WPARAM _param){

	if (_param == VK_UP || _param == VK_DOWN) {

		FPitchX(_param);
	}
	if (_param == VK_RIGHT || _param == VK_LEFT) {

		FRollY(_param);
	}
	if (_param == 'z' || _param == 'Z' || _param == 'c' || _param == 'C') {

		FYawZ(_param);
	}
	else {

		FMove(_param);
	}
}*/