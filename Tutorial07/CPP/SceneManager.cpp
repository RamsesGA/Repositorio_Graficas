#include "../Encabezados/SceneManager.h"

SCENEMANAGER::SCENEMANAGER(){}

SCENEMANAGER::~SCENEMANAGER(){}

void SCENEMANAGER::AddMesh(MESH* newMesh){

	newMesh->SetSceneID(m_ID);
	m_ID++;
	m_MeshInScene.push_back(newMesh);
}

MESH*& SCENEMANAGER::GetMesh(int meshID){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		if (m_MeshInScene[i]->GetSceneID() == meshID)
			return m_MeshInScene[i];
	}
}

void SCENEMANAGER::Update(){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		m_MeshInScene[i]->Update();
	}
}

void SCENEMANAGER::Render(ClaseDeviceContext *_deviceCon, ClaseBuffer* _buff, ClaseDevice* _dev){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		m_MeshInScene[i]->Render(_deviceCon, _buff, _dev);
	}
}

void SCENEMANAGER::Shutdown(){

	for (size_t i = 0; i < m_MeshInScene.size(); i++){

		m_MeshInScene[i]->Delete();
	}
}