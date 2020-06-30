#include "..\Encabezados\GraphicApi.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Encabezados/stb_image.h"

GraphicApi::GraphicApi(){}

GraphicApi::~GraphicApi(){}

/// function for loading the DIRECTX mesh
bool GraphicApi::ChargeMesh(const char* _meshPath, SCENEMANAGER* _sceneManager, const aiScene* _model, ClaseDeviceContext _devCont, Assimp::Importer *_importer, ClaseDevice* _dev){

	/// We import the mesh and save it in a temporary variable
	_model = _importer->ReadFile
											 (_meshPath,
											 aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded |
											 aiProcess_FindInstances |
											 aiProcess_ValidateDataStructure |
											 aiProcess_OptimizeMeshes |
											 aiProcess_Debone |
											 0);

	/// Condition to check if the reading was corrects
	if (_model == nullptr){

		std::string toAdvice = "\nCouldn't find the object in the direcction:\n";
		toAdvice += _meshPath;
		return false;
	}

	/// We create new resources
	MESH* newmesh = new MESH;
	char* token = NULL;
	char* nextToken = NULL;
	std::string path = _meshPath;

	/// We look for the textures in the project folder
	token = strtok_s((char*)path.c_str(), "/", &nextToken);

	std::string teturesFolder = token;
	teturesFolder += "\\";
	token = (char*)teturesFolder.c_str();
	newmesh->m_Materials->m_Diroftextures = token;

	std::string dirName = newmesh->m_Materials->m_Diroftextures;

	/// We send the following functions to be able to assign the data in their respective place
	MeshRead(_model, newmesh, 0, _dev);
	ReadTextureMesh(_model, newmesh, 0, _dev);
	_sceneManager->AddMesh(newmesh);

	/// Finally we check and start generating the resources of the obtained mesh
	if (_model->mNumMeshes > 1){

		for (unsigned int i = 1; i < _model->mNumMeshes; i += (unsigned int)1){

			MESH* childmesh = new MESH;
			childmesh->SetParent(newmesh);
			newmesh->AddChildren(childmesh);
			childmesh->m_Materials->m_Diroftextures = dirName;
			MeshRead(_model, childmesh, i, _dev);
			ReadTextureMesh(_model, childmesh, i, _dev);
			_sceneManager->AddMesh(childmesh);
		}
	}
	return true;
}

/// DIRECTX mesh reading function
void GraphicApi::MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex, ClaseDevice* _dev){
	
	/// We generate the necessary resources to read a mesh
	std::vector <std::uint32_t> indis;
	indis.reserve(_model->mMeshes[_meshIndex]->mNumFaces * 3);
	int numVertex = _model->mMeshes[_meshIndex]->mNumVertices;
	int numVIndex = (_model->mMeshes[_meshIndex]->mNumFaces * 3);

	/// We store the indices in our vector
	for (std::uint32_t faceIdx = 0u; faceIdx < _model->mMeshes[_meshIndex]->mNumFaces; faceIdx++)
	{
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[0u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[1u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[2u]);
	}

	SimpleVertex* meshVertex = new SimpleVertex[numVertex];
	WORD* meshIndex = new WORD[numVIndex];

	/// We assign the vertices and their positions in their respective place
	for (int i = 0; i < numVertex; i++)
	{
		meshVertex[i].msPos.x = _model->mMeshes[_meshIndex]->mVertices[i].x;
		meshVertex[i].msPos.y = _model->mMeshes[_meshIndex]->mVertices[i].y;
		meshVertex[i].msPos.z = _model->mMeshes[_meshIndex]->mVertices[i].z;

		meshVertex[i].msNormal.x = _model->mMeshes[_meshIndex]->mNormals[i].x;
		meshVertex[i].msNormal.y = _model->mMeshes[_meshIndex]->mNormals[i].y;
		meshVertex[i].msNormal.z = _model->mMeshes[_meshIndex]->mNormals[i].z;

		meshVertex[i].msBinormal.x = _model->mMeshes[_meshIndex]->mBitangents[i].x;
		meshVertex[i].msBinormal.y = _model->mMeshes[_meshIndex]->mBitangents[i].y;
		meshVertex[i].msBinormal.z = _model->mMeshes[_meshIndex]->mBitangents[i].z;

		meshVertex[i].msTangent.x = _model->mMeshes[_meshIndex]->mTangents[i].x;
		meshVertex[i].msTangent.y = _model->mMeshes[_meshIndex]->mTangents[i].y;
		meshVertex[i].msTangent.z = _model->mMeshes[_meshIndex]->mTangents[i].z;
		
		meshVertex[i].texcoord.x = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].x;
		meshVertex[i].texcoord.y = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].y;
	}

	_mesh->SetVertex(meshVertex, numVertex);

#ifdef D3D11
	ClaseBuffer::createVertexBuffer(numVertex, _model, meshVertex, _mesh->GetVertexBuffer()->m_BufferD3D11, _dev);
#endif // D3D11

	for (int i = 0; i < numVIndex; i++)
	{
		meshIndex[i] = (WORD)indis[i];
	}
	_mesh->SetIndexList(meshIndex, numVIndex);

#ifdef D3D11
	ClaseBuffer::createIndexBuffer(numVIndex, _model, meshIndex, _mesh->GetIndexBuffer()->m_BufferD3D11, _dev);
#endif // D3D11
}

/// function to read DIRECTX mesh texture
void GraphicApi::ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex, ClaseDevice* _dev) {

	const aiMaterial* pMaterial = _model->mMaterials[_model->mMeshes[_meshIndex]->mMaterialIndex];

	/// Difuse texture
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

		aiString Path;
		_mesh->m_Materials->m_HasDifuse = true;

		/// Within this condition we will look for the extension and format of the textures, in order to assign it to the corresponding mesh
		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

			_mesh->m_Materials->m_DifuseName = _mesh->m_Materials->m_Diroftextures;
			_mesh->m_Materials->m_DifuseName += (std::string) Path.data;

			wchar_t wtext[50];
			char* token = NULL;
			char* nextToken = NULL;
			token = strtok_s((char*)_mesh->m_Materials->m_DifuseName.c_str(), ".", &nextToken);
			std::string pngextencion = token;
			pngextencion += ".png";
			token = (char*)pngextencion.c_str();

			LPCSTR direcTextur = token;
			LPCWSTR dir = (LPCWSTR)pngextencion.c_str();

			int stringLength = MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), 0, 0);
			std::wstring wstr(stringLength, 0);
			MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), &wstr[0], stringLength);
			dir = (LPCWSTR)wstr.c_str();

#ifdef D3D11
			D3DX11CreateShaderResourceViewFromFile(_dev->g_pd3dDeviceD3D11, dir, NULL, NULL, &_mesh->m_Materials->m_TexDif, NULL);
#endif // D3D11
		}
	}
}




#ifdef OPENGL
bool GraphicApi::ChargeMesh(const char* _meshPath, const aiScene* _model, SCENEMANAGER* _sceneManager) {	/// function for loading the OPENGL mesh

	/// We import the mesh and save it in a temporary variable
	_model = m_Importer->ReadFile
	(
		_meshPath,
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_FindInstances |
		aiProcess_ValidateDataStructure |
		aiProcess_OptimizeMeshes |
		aiProcess_Debone |
		0
	);

	/// Condition to check if the reading was corrects
	if (_model == nullptr) {

		std::string toAdvice = "\nCouldn't find the object in the direcction:\n";
		toAdvice += _meshPath;
		return false;
	}

	/// We create new resources
	MESH* newmesh = new MESH;
	char* token = NULL;
	char* nextToken = NULL;
	std::string path = _meshPath;

	/// We look for the textures in the project folder
	token = strtok_s((char*)path.c_str(), "/", &nextToken);

	std::string teturesFolder = token;
	teturesFolder += "\\";
	token = (char*)teturesFolder.c_str();
	newmesh->m_Materials->m_Diroftextures = token;

	std::string dirName = newmesh->m_Materials->m_Diroftextures;

	/// We send the following functions to be able to assign the data in their respective place
	MeshRead(_model, newmesh, 0);
	ReadTextureMesh(_model, newmesh, 0);
	_sceneManager->AddMesh(newmesh);

	///
	/// Finally we check and start generating the resources of the obtained mesh
	///
	if (_model->mNumMeshes > 1) {

		for (unsigned int i = 1; i < _model->mNumMeshes; i += (unsigned int)1) {

			MESH* childmesh = new MESH;
			childmesh->SetParent(newmesh);
			newmesh->AddChildren(childmesh);
			childmesh->m_Materials->m_Diroftextures = dirName;
			MeshRead(_model, childmesh, i);
			ReadTextureMesh(_model, childmesh, i);
			_sceneManager->AddMesh(childmesh);
		}
	}
	return true;
}

const aiNodeAnim* GraphicApi::FindNodeAnimations(const aiAnimation* _animation, const std::string _node) {	//Función para poder encontrar y validar la animación

	for (int i = 0; i < _animation->mNumChannels; i++) {

		const aiNodeAnim* tempNode = _animation->mChannels[i];
		if (std::string(tempNode->mNodeName.data) == _node) {

			return tempNode;
		}
	}
	return nullptr;
}

void GraphicApi::NodeHierarchy(const aiNode* _node, const aiScene* _model, const aiMatrix4x4 _parentTransform, float _animationTime){

	std::string nodeNames(_node->mName.data);
	const aiAnimation* animation = _model->mAnimations[0];
	const aiNodeAnim* nodeAnim = FindNodeAnimations(animation, nodeNames);

	aiMatrix4x4 nodeTransform = _node->mTransformation;
	aiMatrix4x4 globalTransform = _parentTransform * nodeTransform;

	for (int i = 0; i < _node->mNumChildren; i++) {	//Busca el número de hijos del nodo padre y mandamos recursivo hasta que termine todo el árbol

		BonesMatrix tempBone;
		tempBone.boneOffSet = _node->mChildren[i]->mTransformation;
		m_vectorBonesMatrix.push_back(tempBone);
		NodeHierarchy(_node->mChildren[i], _model, globalTransform, _animationTime);
	}
}

void GraphicApi::MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex){	/// function for reading the OPENGL mesh

	/// We generate the necessary resources to read a mesh
	std::vector <std::uint32_t> indis;
	indis.reserve(_model->mMeshes[_meshIndex]->mNumFaces * 3);
	int numVertex = _model->mMeshes[_meshIndex]->mNumVertices;
	int numVIndex = (_model->mMeshes[_meshIndex]->mNumFaces * 3);
	
	//Contamos la cantidad de huesos del objeto cargado
	m_numBones = _model->mMeshes[_meshIndex]->mNumBones;

	//Ciclo para poder guardar los huesos y sus pesos correspondientes
	for (int i = 0; i < m_numBones; i++) {

		VertexBoneData tempVBoneData;
		tempVBoneData.AddBonesData(_model->mMeshes[_meshIndex]->mBones[i]->mWeights->mVertexId, _model->mMeshes[_meshIndex]->mBones[i]->mWeights->mWeight);
		_mesh->m_structVertexBoneData.push_back(tempVBoneData);
	}

	/// We store the indices in our vector
	for (std::uint32_t faceIdx = 0u; faceIdx < _model->mMeshes[_meshIndex]->mNumFaces; faceIdx++){

		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[0u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[1u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[2u]);
	}

	SimpleVertex* meshVertex = new SimpleVertex[numVertex];
	WORD* meshIndex = new WORD[numVIndex];

	/// We assign the vertices and their positions in their respective place
	for (int i = 0; i < numVertex; i++){

		meshVertex[i].msPos.x = _model->mMeshes[_meshIndex]->mVertices[i].x;
		meshVertex[i].msPos.y = _model->mMeshes[_meshIndex]->mVertices[i].y;
		meshVertex[i].msPos.z = _model->mMeshes[_meshIndex]->mVertices[i].z;
		
		meshVertex[i].msNormal.x = _model->mMeshes[_meshIndex]->mNormals[i].x;
		meshVertex[i].msNormal.y = _model->mMeshes[_meshIndex]->mNormals[i].y;
		meshVertex[i].msNormal.z = _model->mMeshes[_meshIndex]->mNormals[i].z;

		meshVertex[i].texcoord.x = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].x;
		meshVertex[i].texcoord.y = _model->mMeshes[_meshIndex]->mTextureCoords[0][i].y;
	}

	//
	aiMatrix4x4 Identity;
	const aiAnimation* tempAnim = _model[_meshIndex].mAnimations[_meshIndex];	//Va a almacenar la animación
	std::string nodeNames = _model[_meshIndex].mAnimations[_meshIndex]->mChannels[_meshIndex]->mNodeName.C_Str();
	const aiNodeAnim* tempNodeAnim = FindNodeAnimations(tempAnim, nodeNames);
	float modelTime = _model[_meshIndex].mAnimations[_meshIndex]->mDuration;

	NodeHierarchy(_model->mRootNode, _model, Identity, modelTime);

	_mesh->SetVertex(meshVertex, numVertex);

	ClaseBuffer::createVertexBuffer(numVertex, meshVertex, _mesh->GetVertexBuffer()->m_ID);

	for (int i = 0; i < numVIndex; i++){

		meshIndex[i] = (WORD)indis[i];
	}

	_mesh->SetIndexList(meshIndex, numVIndex);

	ClaseBuffer::createIndexBuffer(numVIndex, meshIndex, _mesh->GetIndexBuffer()->m_ID);
}

void GraphicApi::ReadTextureMesh(const aiScene* _model, MESH* _mesh, int _meshIndex){	/// function to read OPENGL mesh texture

	const aiMaterial* pMaterial = _model->mMaterials[_model->mMeshes[_meshIndex]->mMaterialIndex];

	/// Difuse texture
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

		aiString Path;
		_mesh->m_Materials->m_HasDifuse = true;

		/// Within this condition we will look for the extension and format of the textures, in order to assign it to the corresponding mesh
		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

			_mesh->m_Materials->m_DifuseName = _mesh->m_Materials->m_Diroftextures;
			_mesh->m_Materials->m_DifuseName += (std::string) Path.data;

			wchar_t wtext[50];
			char* token = NULL;
			char* nextToken = NULL;
			token = strtok_s((char*)_mesh->m_Materials->m_DifuseName.c_str(), ".", &nextToken);
			std::string pngextencion = token;
			pngextencion += ".png";
			token = (char*)pngextencion.c_str();

			LPCSTR direcTextur = token;
			LPCWSTR dir = (LPCWSTR)pngextencion.c_str();

			int stringLength = MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), 0, 0);
			std::wstring wstr(stringLength, 0);
			MultiByteToWideChar(CP_ACP, 0, pngextencion.data(), pngextencion.length(), &wstr[0], stringLength);
			dir = (LPCWSTR)wstr.c_str();

			/// We generate the textures and the bind with the functions integrated by GLFWs
			glGenTextures(1, &_mesh->m_Materials->m_TextureId);
			glBindTexture(GL_TEXTURE_2D, _mesh->m_Materials->m_TextureId);

			/// Set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			/// Load and generate the texture
			int width, height, nrChannels;
			unsigned char* data = stbi_load(pngextencion.c_str(), &width, &height, &nrChannels, 0);

			if (data) {

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {

				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}
#endif // OPENGL