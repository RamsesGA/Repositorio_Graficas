#pragma once
#include "../assimp/Importer.hpp"
#include "../assimp/scene.h"
#include "../assimp/postprocess.h"
#include "../assimp/matrix4x4.h"
#include "../assimp/cimport.h"
#include "ClaseDeviceContext.h"
#include <string>
#include "Mesh.h"
#include "SceneManager.h"

class GraphicApi {

	public:
		GraphicApi();
		~GraphicApi();

		const aiScene* m_Model = new const aiScene();
		Assimp::Importer* m_Importer=new Assimp::Importer();

		bool
		ChargeMesh(const char* _meshPath, SCENEMANAGER* _sceneManager, const aiScene* _model, ClaseDeviceContext _devCont, Assimp::Importer* _importer, ClaseDevice* _dev);

		bool
		ChargeMesh(const char* _meshPath, const aiScene* _model, SCENEMANAGER* _sceneManager);

	private:
		void
		MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex, ClaseDevice* _dev);

		void
		MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex);

		void
		ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex, ClaseDevice* _dev);

		void
		ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex);
};