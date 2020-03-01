#include "Camera.h"

//-----
Camera::Camera(){
	
	m_Desc.s_Widht = 0.0f;
	m_Desc.s_Height = 0.0f;
	m_Desc.s_Near = 0.0f;
	m_Desc.s_Far = 0.0f;
	m_Desc.s_FoV = 0.0f;
	
	m_Front = mathfu::float3(0.0f, 0.0f, 0.0f);
	m_Up = mathfu::float3(0.0f, 0.0f, 0.0f);
	m_Right = mathfu::float3(0.0f, 0.0f, 0.0f);
	m_Desc.s_Eye = mathfu::float3(0.0f, 0.0f, 0.0f);
	m_Desc.s_At = mathfu::float3(0.0f, 0.0f, 0.0f);
}

//-----
Camera::~Camera(){}

//-----
int Camera::Init(CameraDescriptor _cameraDesc){

	m_Desc = _cameraDesc;

	SetWidht(m_Desc.s_Widht);
	SetHeight(m_Desc.s_Height);
	SetNear(m_Desc.s_Near);
	SetFar(m_Desc.s_Far);
	SetFoV(m_Desc.s_FoV);
	SetEye(m_Desc.s_Eye);
	SetAt(m_Desc.s_At);
	SetUp(m_Desc.s_Up);

	CreateView();
	GenerateProjectionMatrix();
	UpdateVM();

	return 0;
}

//-----
mathfu::float3 Camera::GetMUp(){

	return m_Up;
}

//-----
mathfu::float3 Camera::GetMFront(){

	return m_Front;
}

//-----
mathfu::float3 Camera::GetMRight(){

	return m_Right;
}

//-----
void Camera::SetWidht(float _widht){

	m_Desc.s_Widht = _widht;
}
float Camera::GetWidht(){

	return m_Desc.s_Widht;
}

//-----
void Camera::SetHeight(float _height){

	m_Desc.s_Height = _height;
}
float Camera::GetHeight(){

	return m_Desc.s_Height;
}

//-----
void Camera::SetNear(float _near){

	m_Desc.s_Near = _near;
}
float Camera::GetNear(){

	return m_Desc.s_Near;
}

//-----
void Camera::SetFar(float _far){

	m_Desc.s_Far = _far;
}
float Camera::GetFar(){

	return m_Desc.s_Far;
}

//-----
void Camera::SetFoV(float _fOv){

	m_Desc.s_FoV = _fOv;
}
float Camera::GetFoV(){

	return m_Desc.s_FoV;
}

//-----
void Camera::SetEye(mathfu::float3 _vec3){

	m_Desc.s_Eye = _vec3;
}
mathfu::float3 Camera::GetEye(){

	return m_Desc.s_Eye;
}

//-----
void Camera::SetAt(mathfu::float3 _vec3){

	m_Desc.s_At = _vec3;
}
mathfu::float3 Camera::GetAt(){

	return m_Desc.s_At;
}

//-----
void Camera::SetUp(mathfu::float3 _vec3){

	m_Desc.s_Up = _vec3;
}
mathfu::float3 Camera::GetUp(){

	return m_Desc.s_Up;
}

//-----
void Camera::UpdateVM(){

	//m_Desc.s_Eye = {m_Projection.data_[0].w, m_Projection.data_[1].w , m_Projection.data_[2].w };
	m_Right = { m_View.data_[0].x, m_View.data_[0].y, m_View.data_[0].z };
	m_Up	= { m_View.data_[1].x, m_View.data_[1].y, m_View.data_[1].z };
	m_Front = { m_View.data_[2].x, m_View.data_[2].y, m_View.data_[2].z };
}

//-----
void Camera::GenerateProjectionMatrix(){

#ifdef D3D11
	m_MProjection = XMMatrixPerspectiveFovLH(m_Desc.s_FoV, m_Desc.s_Widht / m_Desc.s_Height, m_Desc.s_Near, m_Desc.s_Far);

	m_Projection =
	{
		m_MProjection._11, m_MProjection._12, m_MProjection._13, m_MProjection._14,
		m_MProjection._21, m_MProjection._22, m_MProjection._23, m_MProjection._24,
		m_MProjection._31, m_MProjection._32, m_MProjection._33, m_MProjection._34,
		m_MProjection._41, m_MProjection._42, m_MProjection._43, m_MProjection._44
	};

	m_Projection = m_Projection.Transpose();
#endif
}

//-----
void Camera::CreateView() {

	//Usamos left hand
	m_Front = m_Desc.s_At - m_Desc.s_Eye;
	m_Front.Normalize();

	m_Right = mathfu::cross(m_Desc.s_Up, m_Front);
	m_Right.Normalize();

	m_Up = mathfu::cross(m_Front, m_Right);
	//m_Up.Normalize();

	m_Axis = 
	{
		m_Right.x,	m_Right.y,	m_Right.z,	0,
		m_Up.x,		m_Up.y,		m_Up.z,		0,
		m_Front.x,	m_Front.y,	m_Front.z,	0,
		0,			0,			0,			1
	};

	m_Position =
	{
		1,	0,	0, -m_Desc.s_Eye.x,
		0,	1,	0, -m_Desc.s_Eye.y,
		0,	0,	1, -m_Desc.s_Eye.z,
		0,	0,	0,	1
	};

	m_Position *= m_Axis;
	m_View = m_Position; //Ya está la matriz view
}

//-----
void Camera::Move(WPARAM _param){

	if (_param == 'w' || _param == 'W') {

		m_Desc.s_Eye += m_Front;
	}
	else if (_param == 's' || _param == 'S') {

		m_Desc.s_Eye -= m_Front;
	}
	else if (_param == 'a' || _param == 'A') {

		m_Desc.s_Eye -= m_Right;
	}
	else if (_param == 'd' || _param == 'D') {

		m_Desc.s_Eye += m_Right;
	}
	else if (_param == 'q' || _param == 'Q') {

		m_Desc.s_Eye += m_Up;
	}
	else if (_param == 'e' || _param == 'E') {

		m_Desc.s_Eye -= m_Up;
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
		1,	0,	0, -m_Desc.s_Eye.x,
		0,	1,	0, -m_Desc.s_Eye.y,
		0,	0,	1, -m_Desc.s_Eye.z,
		0,	0,	0,	1
	};


	m_Position *= m_Axis;
	m_View = m_Position; //Ya está la matriz view
	UpdateVM();
}

//-----
void Camera::PitchX(WPARAM _param){

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

		//rot *= m_View;
		//m_View = rot;
	}
	else if (_param == VK_DOWN) {

		rot = 
		{
			1,	0,					0,					0,
			0,	cosf(-speedrot),	-sinf(-speedrot),	0,
			0,	sinf(-speedrot),	cosf(-speedrot),	0,
			0,	0,					0,					1
		};
		//rot *= m_View;
		//m_View = rot;
	}
	m_View *= rot;

	UpdateVM();
}  

//-----
void Camera::YawZ(WPARAM _param){

	mathfu::float4x4 rot;
	float speedrot = 0.10f;

	if (_param == 'z'|| _param == 'Z') {

		rot =
		{
			cosf(speedrot),		0,	sinf(speedrot),	0,
			0,					1,	0,				0,
			-sinf(speedrot),	0,	cosf(speedrot),	0,
			0,					0,	0,				1
		};
		//rot *= m_View;
		//m_View = rot;
	}
	else if (_param == 'c' || _param == 'C') {

		rot = 
		{
			cosf(-speedrot),		0,	sinf(-speedrot),	0,
			0,						1,	0,					0,
			-sinf(-speedrot),		0,	cosf(-speedrot),	0,
			0,						0,	0,					1
		};
		//rot *= m_View;
		//m_View = rot;
	}
	m_View *= rot;

	UpdateVM();
}

//-----
void Camera::RollY(WPARAM _param){

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
		//rot *= m_View;
		//m_View = rot;
	}
	else if (_param == VK_LEFT) {

		rot = 
		{
			cosf(-speedrot),	-sinf(-speedrot),		0,		0,
			sinf(-speedrot),	cosf(-speedrot),		0,		0,
			0,				0,							1,		0,
			0,				0,							0,		1
		};
		//rot *= m_View;
		//m_View = rot;
	}
	m_View *= rot;

	UpdateVM();
}

//-----
void Camera::MouseRotation(){

	mathfu::float2 FirstPos;
	mathfu::float2 SecondPos;
	float SpeedRot = 0.0015f;
	
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

	if (FirstPos.x < OriginalMousePos.x) {

			Yaw =
			{
				cosf(SpeedRot),		0,	sinf(SpeedRot),	0,
				0,					1,	0,				0,
				-sinf(SpeedRot),	0,	cosf(SpeedRot),	0,
				0,					0,	0,				1
			};
	}
	if (FirstPos.x > OriginalMousePos.x) {

			Yaw =
			{
				cosf(-SpeedRot),		0,	sinf(-SpeedRot),	0,
				0,						1,	0,					0,
				-sinf(-SpeedRot),		0,	cosf(-SpeedRot),	0,
				0,						0,	0,					1
			};
	}
	if (SecondPos.y < OriginalMousePos.y) {
			
			Pitch =
			{
				1,	0,				0,					0,
				0,	cosf(SpeedRot),	-sinf(SpeedRot),	0,
				0,	sinf(SpeedRot),	cosf(SpeedRot),		0,
				0,	0,				0,					1
			};
	}
	if (SecondPos.y > OriginalMousePos.y) {

			Pitch =
			{
				1,	0,					0,					0,
				0,	cosf(-SpeedRot),	-sinf(-SpeedRot),	0,
				0,	sinf(-SpeedRot),	cosf(-SpeedRot),	0,
				0,	0,					0,					1
			};
	}
	
	//SetCursorPos(OriginalMousePos.x, OriginalMousePos.y);
	m_View *= Yaw;
	UpdateVM();
	m_View *= Pitch;
	UpdateVM();
}

//-----
void Camera::inputs(WPARAM _param){

	if (_param == VK_UP || _param == VK_DOWN){

		PitchX(_param);
	}
	if (_param == VK_RIGHT || _param == VK_LEFT) {

		RollY(_param);
	}
	if (_param == 'z' || _param == 'Z' || _param == 'c' || _param == 'C') {

		YawZ(_param);
	}
	else{

		Move(_param);
	}
}