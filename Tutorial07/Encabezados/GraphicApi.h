#pragma once
#include "ClaseDeviceContext.h"
#include <string>
#include "Mesh.h"
#include "SceneManager.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include "assimp/cimport.h"

///
/// Class with the necessary functions to load the model
///
class GraphicApi {

	public:
		GraphicApi();
		~GraphicApi();


		//Variables
		unsigned int m_numBones;
		std::vector<BonesMatrix> m_vectorBonesMatrix;

		///
		/// DIRECTX Methods
		///
		bool
		ChargeMesh(const char* _meshPath, SCENEMANAGER* _sceneManager, const aiScene* _model, ClaseDeviceContext _devCont, Assimp::Importer* _importer, ClaseDevice* _dev);
		
		///
		/// ASSIMP
		///
		const aiScene* m_Model = new const aiScene();
		Assimp::Importer* m_Importer=new Assimp::Importer();

		///
		/// DIRECTX Methods
		///
		void
		MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex, ClaseDevice* _dev);

		void
		ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex, ClaseDevice* _dev);

		///
		/// OPENGL Methods
		///
#ifdef OPENGL
		void MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex);

		void ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex);

		bool ChargeMesh(const char* _meshPath, const aiScene* _model, SCENEMANAGER* _sceneManager);

		const aiNodeAnim* FindNodeAnimations(const aiAnimation* _animation, const std::string _node);

		void NodeHierarchy(const aiNode* _node, const aiScene* _model, const aiMatrix4x4 _parentTransform, float _animationTime);
#endif // OPENGL
};