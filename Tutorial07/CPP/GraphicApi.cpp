#include "..\Encabezados\GraphicApi.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Encabezados/stb_image.h"

GraphicApi::GraphicApi(){}

GraphicApi::~GraphicApi(){}

/// function for loading the DIRECTX mesh
const aiScene* GraphicApi::ChargeMesh(const char* _meshPath, SCENEMANAGER* _sceneManager, const aiScene* _model, ClaseDeviceContext _devCont, Assimp::Importer *_importer, ClaseDevice* _dev, char* _diffuse, char* _normal, char* _specular){

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

	//Nuevo, guardamos los datos
	if (_diffuse != ""){

		wchar_t text[50];
		mbstowcs(text, _diffuse, strlen(_diffuse) + 1);
		LPWSTR srcFile = text;
		D3DX11CreateShaderResourceViewFromFile(_dev->g_pd3dDeviceD3D11, srcFile, NULL, NULL, &newmesh->m_Materials->m_TexDif, NULL);
	}
	if (_normal != ""){

		wchar_t text[50];
		mbstowcs(text, _normal, strlen(_normal) + 1);
		LPWSTR srcFile = text;
		D3DX11CreateShaderResourceViewFromFile(_dev->g_pd3dDeviceD3D11, srcFile, NULL, NULL, &newmesh->m_Materials->m_TexNorm, NULL);
	}
	if (_specular != ""){

		wchar_t text[50];
		mbstowcs(text, _specular, strlen(_specular) + 1);
		LPWSTR srcFile = text;
		D3DX11CreateShaderResourceViewFromFile(_dev->g_pd3dDeviceD3D11, srcFile, NULL, NULL, &newmesh->m_Materials->m_TexSpec, NULL);
	}

	_sceneManager->AddMesh(newmesh);

	/// Finally we check and start generating the resources of the obtained mesh
	if (_model->mNumMeshes > 1){

		for (unsigned int i = 1; i < _model->mNumMeshes; i += (unsigned int)1){

			MESH* childmesh = new MESH;
			childmesh->SetParent(newmesh);

			newmesh->AddChildren(childmesh);

			childmesh->m_Materials->m_Diroftextures = dirName;

			MeshRead(_model, childmesh, i, _dev);

			_sceneManager->AddMesh(childmesh);
		}
	}
	return _model;
}

void GraphicApi::BoneTransform(float _time, std::vector<mathfu::float4x4>& _transform, const aiScene* _model, SCENEMANAGER* _sceneManager){

	mathfu::float4x4 identity;
	identity = identity.Identity();

	int allBones = 0;

	for (int i = 0; i < _sceneManager->m_MeshInScene.size(); i++) {

		allBones += _sceneManager->m_MeshInScene.at(i)->m_numBones;
	}

	float ticksPerSecond = _model->mAnimations[0]->mTicksPerSecond != 0 ? _model->mAnimations[0]->mTicksPerSecond : 25.0f;

	float timeInTicks = _time * ticksPerSecond;

	float animationTime = fmod(timeInTicks, _model->mAnimations[0]->mDuration);

	mathfu::float4x4 temp;
	temp = temp.Identity();

	for (int i = 0; i < _sceneManager->m_MeshInScene.size(); i++) {

		NodeHierarchy(_model->mRootNode, _model, temp, animationTime, _sceneManager->m_MeshInScene[i]);
	}

	_transform.resize(allBones);

	for (int i = 0; i < allBones; i++) {

		for (int j = 0; j < _sceneManager->m_MeshInScene.size(); j++) {

			for (int k = 0; k < _sceneManager->m_MeshInScene[j]->m_boneInfo.size(); k++) {

				_transform[i] = _sceneManager->m_MeshInScene[j]->m_boneInfo[k].finalTransformation.Transpose();
				i++;
			}
		}
	}
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

void GraphicApi::NodeHierarchy(const aiNode* _node, const aiScene* _model, mathfu::float4x4 _parentTransform, float _animationTime, MESH* _mesh) {

	std::string nodeNames(_node->mName.data);
	const aiAnimation* animation = _model->mAnimations[0];
	
	//Transponemos
	mathfu::float4x4 matrixTransform = {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	matrixTransform(0.0, 0.0) = _node->mTransformation.a1;
	matrixTransform(0.0, 1.0) = _node->mTransformation.a2;
	matrixTransform(0.0, 2.0) = _node->mTransformation.a3;
	matrixTransform(0.0, 3.0) = _node->mTransformation.a4;

	matrixTransform(1.0, 0.0) = _node->mTransformation.b1;
	matrixTransform(1.0, 1.0) = _node->mTransformation.b2;
	matrixTransform(1.0, 2.0) = _node->mTransformation.b3;
	matrixTransform(1.0, 3.0) = _node->mTransformation.b4;

	matrixTransform(2.0, 0.0) = _node->mTransformation.c1;
	matrixTransform(2.0, 1.0) = _node->mTransformation.c2;
	matrixTransform(2.0, 2.0) = _node->mTransformation.c3;
	matrixTransform(2.0, 3.0) = _node->mTransformation.c4;
	
	matrixTransform(3.0, 0.0) = _node->mTransformation.d1;
	matrixTransform(3.0, 1.0) = _node->mTransformation.d2;
	matrixTransform(3.0, 2.0) = _node->mTransformation.d3;
	matrixTransform(3.0, 3.0) = _node->mTransformation.d4;

	//matrixTransform = matrixTransform.FromType(&_node->mTransformation.a1);
	matrixTransform = matrixTransform.Transpose();

	const aiNodeAnim* nodeAnim = FindNodeAnimations(animation, nodeNames);

	if (nodeAnim) {

		//Interpolate scaling and generate scaling transformation matrix
		aiVector3D scaling;

		InterpolatedScaling(scaling, _animationTime, nodeAnim);

		mathfu::float4x4 scalingMatrix;
		scalingMatrix = scalingMatrix.Identity();


		//Translate the values from aiVector3D to Vec3
		mathfu::float3 scalingMathfu(scaling.x, scaling.y, scaling.z);
		scalingMatrix = scalingMatrix.FromScaleVector(scalingMathfu);

		//Interpolate rotation and generate rotation transform matrix
		aiQuaternion rotationQuaternion;
		InterpolatedRotation(rotationQuaternion, _animationTime, nodeAnim);
		mathfu::Quaternion<float> rotationQuaternionMathfu(rotationQuaternion.w ,rotationQuaternion.x, rotationQuaternion.y, rotationQuaternion.z);
		mathfu::float4x4 rotationMatrix = {

			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0
		};

		//rotationQuaternionMathfu.FromMatrix(rotationMatrix);

		rotationMatrix = rotationQuaternionMathfu.ToMatrix4();

		//Interpolate translation and generate translation tranformation matrix
		aiVector3D translation;
		InterpolatedPosition(translation, _animationTime, nodeAnim);
		mathfu::float4x4 translationMatrix;

		translationMatrix = translationMatrix.Identity();

		translationMatrix(3.0f, 0.0f) = translation.x;
		translationMatrix(3.0f, 1.0f) = translation.y;
		translationMatrix(3.0f, 2.0f) = translation.z;

		//Combine the above transformations
		matrixTransform = translationMatrix * rotationMatrix * scalingMatrix;
	}

	mathfu::float4x4 globalTransformation = {
	
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	globalTransformation = _parentTransform * matrixTransform;

	if (_mesh->m_boneMapping.find(nodeNames) != _mesh->m_boneMapping.end()) {

		int boneIndex = _mesh->m_boneMapping[nodeNames];
		_mesh->m_boneInfo[boneIndex].finalTransformation = globalTransformation * _mesh->m_boneInfo[boneIndex].boneOffSet;
	}

	for (int i = 0; i < _node->mNumChildren; i++) {

		NodeHierarchy(_node->mChildren[i], _model, globalTransformation, _animationTime, _mesh);
	}
}

void GraphicApi::InterpolatedPosition(aiVector3D& _out, float _animationTime, const aiNodeAnim* _nodeAnim){

	if (_nodeAnim->mNumPositionKeys == 1) {

		_out = _nodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int positionIndex = FindPosition(_animationTime, _nodeAnim);
	unsigned int nextPositionIndex = (positionIndex + 1);

	assert(nextPositionIndex < _nodeAnim->mNumPositionKeys);

	float deltaTime = (float)(_nodeAnim->mPositionKeys[nextPositionIndex].mTime - _nodeAnim->mPositionKeys[positionIndex].mTime);
	float factor = (_animationTime - (float)_nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& start = _nodeAnim->mPositionKeys[positionIndex].mValue;
	const aiVector3D& end = _nodeAnim->mPositionKeys[nextPositionIndex].mValue;

	aiVector3D delta = end - start;

	_out = start + factor * delta;
}

void GraphicApi::InterpolatedRotation(aiQuaternion& _out, float _animationTime, const aiNodeAnim* _nodeAnim){

	if (_nodeAnim->mNumRotationKeys == 1) {

		_out = _nodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int rotationIndex = FindRotation(_animationTime, _nodeAnim);
	unsigned int nextRotationIndex = (rotationIndex + 1);

	assert(nextRotationIndex < _nodeAnim->mNumRotationKeys);

	float deltaTime = (float)(_nodeAnim->mRotationKeys[nextRotationIndex].mTime - _nodeAnim->mRotationKeys[rotationIndex].mTime);
	float factor = (_animationTime - (float)_nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiQuaternion& StartRotationQ = _nodeAnim->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& EndRotationQ = _nodeAnim->mRotationKeys[nextRotationIndex].mValue;

	aiQuaternion::Interpolate(_out, StartRotationQ, EndRotationQ, factor);
	_out = _out.Normalize();
}

void GraphicApi::InterpolatedScaling(aiVector3D& _out, float _animationTime, const aiNodeAnim* _nodeAnim){

	if (_nodeAnim->mNumScalingKeys == 1) {

		_out = _nodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int scalingIndex = FindScaling(_animationTime, _nodeAnim);
	unsigned int nextScalingIndex = (scalingIndex + 1);

	assert(nextScalingIndex < _nodeAnim->mNumScalingKeys);

	float deltaTime = (float)(_nodeAnim->mScalingKeys[nextScalingIndex].mTime - _nodeAnim->mScalingKeys[scalingIndex].mTime);
	float factor = (_animationTime - (float)_nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& start = _nodeAnim->mScalingKeys[scalingIndex].mValue;
	const aiVector3D& end = _nodeAnim->mScalingKeys[nextScalingIndex].mValue;

	aiVector3D delta = end - start;
	_out = start + factor * delta;
}

int GraphicApi::FindPosition(float _animationTime, const aiNodeAnim* _nodeAnim){


	for (int i = 0; i < _nodeAnim->mNumPositionKeys - 1; i++) {

		if (_animationTime < (float)_nodeAnim->mPositionKeys[i + 1].mTime) {

			return i;
		}
	}

	assert(0);
	
	return 0;
}

int GraphicApi::FindRotation(float _animationTime, const aiNodeAnim* _nodeAnim){

	assert(_nodeAnim->mNumRotationKeys > 0);

	for (int i = 0; i < _nodeAnim->mNumRotationKeys - 1; i++) {

		if (_animationTime < (float)_nodeAnim->mRotationKeys[i + 1].mTime) {

			return i;
		}
	}

	assert(0);

	return 0;
}

int GraphicApi::FindScaling(float _animationTime, const aiNodeAnim* _nodeAnim) {

	assert(_nodeAnim->mNumScalingKeys > 0);

	for (int i = 0; i < _nodeAnim->mNumScalingKeys - 1; i++) {

		if (_animationTime < (float)_nodeAnim->mScalingKeys[i + 1].mTime) {

			return i;
		}
	}

	assert(0);

	return 0;
}

/// DIRECTX mesh reading function
void GraphicApi::MeshRead(const aiScene* _model, MESH* _mesh, int _meshIndex, ClaseDevice* _dev){
	
	/// We generate the necessary resources to read a mesh
	std::vector <std::uint32_t> indis;
	indis.reserve(_model->mMeshes[_meshIndex]->mNumFaces * 3);
	int numVertex = _model->mMeshes[_meshIndex]->mNumVertices;
	int numVIndex = (_model->mMeshes[_meshIndex]->mNumFaces * 3);

	//Animación
	int numBones = (_model->mMeshes[_meshIndex]->mNumBones);
	_mesh->m_bonesVertex.resize(numVertex);

	SimpleVertex* meshVertex = new SimpleVertex[numVertex];
	WORD* meshIndex = new WORD[numVIndex];

	//Descomentar para regresar a las animaciones
	/*for (int i = 0; i < numBones; i++) {

		int boneIndex = 0;

		//Obtiene el nombre del hueso en base al mesh que tiene en el momento
		std::string boneName(_model->mMeshes[_meshIndex]->mBones[i]->mName.data);

		if (_mesh->m_boneMapping.find(boneName) == _mesh->m_boneMapping.end()) {	//Usamos el mapa y revisamos si el mesh dentro de su mapa tenga el nombre obtenido

			boneIndex = _mesh->m_numBones;
			_mesh->m_numBones++;

			BoneInfo boneInfo;
			_mesh->m_boneInfo.push_back(boneInfo);
		}
		else {	//Si no, lo agrega

			boneIndex = _mesh->m_boneMapping[boneName];
		}

		_mesh->m_boneMapping[boneName] = boneIndex;
		
		_mesh->m_boneInfo[boneIndex].boneOffSet(0.0, 0.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.a1;
		_mesh->m_boneInfo[boneIndex].boneOffSet(0.0, 1.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.a2;
		_mesh->m_boneInfo[boneIndex].boneOffSet(0.0, 2.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.a3;
		_mesh->m_boneInfo[boneIndex].boneOffSet(0.0, 3.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.a4;

		_mesh->m_boneInfo[boneIndex].boneOffSet(1.0, 0.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.b1;
		_mesh->m_boneInfo[boneIndex].boneOffSet(1.0, 1.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.b2;
		_mesh->m_boneInfo[boneIndex].boneOffSet(1.0, 2.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.b3;
		_mesh->m_boneInfo[boneIndex].boneOffSet(1.0, 3.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.b4;

		_mesh->m_boneInfo[boneIndex].boneOffSet(2.0, 0.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.c1;
		_mesh->m_boneInfo[boneIndex].boneOffSet(2.0, 1.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.c2;
		_mesh->m_boneInfo[boneIndex].boneOffSet(2.0, 2.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.c3;
		_mesh->m_boneInfo[boneIndex].boneOffSet(2.0, 3.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.c4;

		_mesh->m_boneInfo[boneIndex].boneOffSet(3.0, 0.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.d1;
		_mesh->m_boneInfo[boneIndex].boneOffSet(3.0, 1.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.d2;
		_mesh->m_boneInfo[boneIndex].boneOffSet(3.0, 2.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.d3;
		_mesh->m_boneInfo[boneIndex].boneOffSet(3.0, 3.0) = _model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.d4;


		//_mesh->m_boneInfo[boneIndex].boneOffSet = _mesh->m_boneInfo[boneIndex].boneOffSet.FromType(&_model->mMeshes[_meshIndex]->mBones[i]->mOffsetMatrix.a1);
		_mesh->m_boneInfo[boneIndex].boneOffSet = _mesh->m_boneInfo[boneIndex].boneOffSet.Transpose();

		for (int j = 0; j < _model->mMeshes[_meshIndex]->mBones[i]->mNumWeights; j++) {

			for (int k = 0; k < 4; k++) {

				meshVertex[_model->mMeshes[_meshIndex]->mBones[i]->mWeights[j].mVertexId].boneId[k] = boneIndex;
				meshVertex[_model->mMeshes[_meshIndex]->mBones[i]->mWeights[j].mVertexId].weights[k] = _model->mMeshes[_meshIndex]->mBones[i]->mWeights[j].mWeight;

			}
		}
	}*/

	/// We store the indices in our vector
	for (std::uint32_t faceIdx = 0u; faceIdx < _model->mMeshes[_meshIndex]->mNumFaces; faceIdx++){

		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[0u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[1u]);
		indis.push_back(_model->mMeshes[_meshIndex]->mFaces[faceIdx].mIndices[2u]);
	}

	/// We assign the vertices and their positions in their respective place
	for (int i = 0; i < numVertex; i++){

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

	for (int i = 0; i < numVIndex; i++){

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