#pragma once
#include "ClaseDeviceContext.h"
#include "ClaseBuffer.h"
#include <vector>
#include "Defines.h"
#include "mathfu/hlsl_mappings.h"
#include <string>
#include "MaterialApi.h"

#include <map>


typedef struct VertexBoneData { //La cantidad de huesos que tienen esos vertex y los pesos de estos

	float boneID[g_NUM_BONES_VERTEX] = { 0 };
	float weights[g_NUM_BONES_VERTEX] = { 0 };

	void AddBonesData(unsigned int _boneId, float _weightBones);
};

///
/// Class with functions for loading and set of Mesh values
///
class MESH
{
	public:

		std::vector <VertexBoneData> m_structVertexBoneData;

		///
		/// Builder and destroyer
		///
		MESH();
		~MESH();
	
		///
		/// Main methods for the class
		///
		void
		Init();

		void
		Update();

		void
		Render(ClaseDeviceContext *_devCont, ClaseBuffer* _bufferData, ClaseDevice* _dev);

		void
		Delete();

		void
		AddChildren(MESH* _newChild);

		void
		SetParent(MESH* _parent);

		void 
		SetVertex(SimpleVertex* MyVertex, int numVertex);

		void 
		SetIndexList(WORD* _index, int numIndex);

		WORD*&
		GetIndexList(SimpleVertex* MyVertex, int numVertex) { return m_MyIndex; };

		SimpleVertex* 
		GetVertex() { return m_MyVertex; };
		
		int
		GetVertexNum() { return m_VertexNum; };
		
		int 
		GetIndexNum() { return m_IndexNum; };
		
		ClaseBuffer*&
		GetVertexBuffer() { return m_VertexBuffer; };
		
#ifdef D3D11
		void
		SetVertexBuffer(ID3D11Buffer* _pVertexB) { m_VertexBuffer->m_BufferD3D11 = _pVertexB; };

		void
		SetIndexBuffer(ID3D11Buffer* _pIndexB) { m_Index->m_BufferD3D11 = _pIndexB; };
#endif // D3D11
		
		ClaseBuffer*&
		GetIndexBuffer() { return m_Index; };
		
		void 
		SetSceneID(int ID) { m_SceneId = ID; };
		
		int 
		GetSceneID() { return m_SceneId; };


		//Variables-----
		std::string		m_DifuceName;
		MaterialApi* m_Materials;


	public:
		
		///
		/// Members
		///
		MESH*				m_Parent;
		std::vector<MESH*>	m_Children;
		mathfu::float4		m_Transform;
		ClaseBuffer*		m_VertexBuffer	= nullptr;
		ClaseBuffer*		m_Index			= nullptr;
		SimpleVertex*		m_MyVertex		= nullptr;
		WORD*				m_MyIndex		= nullptr;
		int					m_VertexNum		= 0;
		int					m_IndexNum		= 0;
		int					m_SceneId		= -1;
		CBChangesEveryFrame m_MeshData;

		//Variables para la animación
		int m_numBones;
		std::map<std::string, int> m_boneMapping;
		std::vector<VertexBoneData> m_bonesVertex;
		std::vector<BoneInfo> m_boneInfo;
};