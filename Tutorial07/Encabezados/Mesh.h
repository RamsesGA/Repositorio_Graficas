#pragma once
#include "ClaseDeviceContext.h"
#include "ClaseBuffer.h"
#include <vector>
#include "Defines.h"
#include "mathfu/hlsl_mappings.h"
#include <string>
#include "MaterialApi.h"

class MESH
{
	public:
		MESH();
		~MESH();
	
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
		
		void 
		SetVertexBuffer(ID3D11Buffer* _pVertexB) { m_VertexBuffer->m_BufferD3D11 = _pVertexB; };
		
		ClaseBuffer*&
		GetIndexBuffer() { return m_Index; };
		
		void 
		SetIndexBuffer(ID3D11Buffer* _pIndexB) { m_Index->m_BufferD3D11 = _pIndexB; };
		
		void 
		SetSceneID(int ID) { m_SceneId = ID; };
		
		int 
		GetSceneID() { return m_SceneId; };


		//Variables-----
		std::string		m_DifuceName;
		MaterialApi*	m_Materials;

	public:
		//Variables-----
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
};