#include "FirstCamera.h"

FirstCamera::FirstCamera(){}

FirstCamera::~FirstCamera(){}

void FirstCamera::UpdateVM() {

	m_Right = { m_View.data_[0].x,	0,				   m_View.data_[0].z };
	m_Up	= { m_View.data_[1].x,	m_View.data_[1].y, m_View.data_[1].z };
	m_Front = { m_View.data_[2].x,	m_View.data_[2].y, m_View.data_[2].z };

	m_Desc.s_At = m_Desc.s_Eye + m_Front;
}

void FirstCamera::CreateView() {

	//Usamos left hand
	m_Front = m_Desc.s_At - m_Desc.s_Eye;
	m_Front.Normalize();

	m_Desc.s_At = m_Desc.s_Eye + m_Front;

	m_Right = mathfu::cross(m_Desc.s_Up, m_Front);
	m_Right = { m_Right.x,0,m_Right.z };
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

void FirstCamera::MouseRotation(){

	mathfu::float2 FirstPos;
	mathfu::float2 SecondPos;
	float SpeedRot = 0.015f;
	float speedAngule = 2;

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

		//Yaw =
		//{
		//	cosf(SpeedRot),		0,	sinf(SpeedRot),	0,
		//	0,					1,	0,				0,
		//	-sinf(SpeedRot),	0,	cosf(SpeedRot),	0,
		//	0,					0,	0,				1
		//};
		m_Desc.s_At -= m_Right * SpeedRot;
		m_Desc.s_Up = m_Up;
		CreateView();
	}
	if (FirstPos.x > OriginalMousePos.x) {

		//Yaw =
		//{
		//	cosf(-SpeedRot),		0,	sinf(-SpeedRot),	0,
		//	0,						1,	0,					0,
		//	-sinf(-SpeedRot),		0,	cosf(-SpeedRot),	0,
		//	0,						0,	0,					1
		//};
		m_Desc.s_At += m_Right * SpeedRot;
		m_Desc.s_Up = m_Up;
		CreateView();
	}

	if (SecondPos.y < OriginalMousePos.y && angule < maxAngule) {

		angule += speedAngule;

		if (angule > maxAngule){

			angule = maxAngule;
		}
		else{

			Pitch =
			{
				1,	0,									0,										0,
				0,	cosf(speedAngule * 3.141516 / 180),	-sinf(speedAngule * 3.141516 / 180),	0,
				0,	sinf(speedAngule * 3.141516 / 180),	cosf(speedAngule * 3.141516 / 180),		0,
				0,	0,									0,										1
			};
		}

	}
	if (SecondPos.y > OriginalMousePos.y&& angule > -maxAngule) {

		angule -= speedAngule;

		if (angule < -maxAngule){

			angule = -maxAngule;
		}
		else{

			Pitch =
			{
				1,	0,										0,										0,
				0,	cosf(-speedAngule * 3.141516 / 180),	-sinf(-speedAngule * 3.141516 / 180),	0,
				0,	sinf(-speedAngule * 3.141516 / 180),	cosf(-speedAngule * 3.141516 / 180),	0,
				0,	0,										0,										1
			};
		}

	}

	SetCursorPos(OriginalMousePos.x, OriginalMousePos.y);
	m_View *= Yaw;
	UpdateVM();
	m_View *= Pitch;
	UpdateVM();
}