#include <../Encabezados/Mesh.h>
#include<sstream> 
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

MESH::MESH(){

	m_VertexBuffer	= new ClaseBuffer;
	m_Index			= new ClaseBuffer;
	m_Materials		= new MaterialApi;
	m_MeshData.mWorld = {
		1,0,0,0.010,
		0,1,0,0.010,
		0,0,1,0.010,
		0,0,0,1
	};
	m_MeshData.vMeshColor = { 1,0,0,1 };
}

MESH::~MESH(){}

//Creación de nuevos buffers
void MESH::Init() {

	m_VertexBuffer	= new ClaseBuffer;
	m_Index			= new ClaseBuffer;
}

//
void MESH::Update(){
}

void MESH::Render(ClaseDeviceContext *_devCont, ClaseBuffer* _bufferData, ClaseDevice* _dev){

	m_MeshData.mWorld = {
		1,0,0,0.0010,
		0,1,0,0.0010,
		0,0,1,0.0010,
		0,0,0,1
	};
	m_MeshData.vMeshColor = { 1,0,0,1 };
	_devCont->g_pImmediateContextD3D11->PSSetShaderResources	(0, 1, &m_Materials->m_TexDif);
	_devCont->g_pImmediateContextD3D11->VSSetShaderResources	(0, 1, &m_Materials->m_TexDif);
	//_devCont->g_pImmediateContextD3D11->UpdateSubresource	(_bufferData->m_BufferD3D11, 0, NULL, &m_MeshData, 0, 0);
	//
	//_devCont->g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &_bufferData->m_BufferD3D11);
	//_devCont->g_pImmediateContextD3D11->PSSetConstantBuffers(2, 1, &_bufferData->m_BufferD3D11);

	std::string title;
	std::string title2;

	std::stringstream ss;

	ss << "mesh ";
	ss << m_SceneId;
	ss >> title;
	ss >> title2;

	title += " ";
	title += title2;
	
	//ImGui::Begin(title.c_str(), 0);
	//ImGui::ColorEdit4("color", m_MeshData.vMeshColor.Vector);
	//ImGui::End();

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	_devCont->g_pImmediateContextD3D11->IASetVertexBuffers
	(	0,
		1,//numero de buffers que estamos utilizando
		&m_VertexBuffer->m_BufferD3D11,//puntero a la lista buffers
		&stride,//un uint que indica el tamaño de un unico vertice
		&offset
	);//un uint que indica el numero del byte en el vertice del que se quiere comenzar a pintar

	_devCont->g_pImmediateContextD3D11->IASetIndexBuffer
	(
		m_Index->m_BufferD3D11,
		DXGI_FORMAT_R16_UINT,
		0
	);

	//Tipo de topologia
	/*Esta segunda función le dice a Direct3D qué tipo de primitiva se usa.*/
	//_devCont.g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	 //Dibuja el búfer de vértices en el búfer posterior 
	_devCont->g_pImmediateContextD3D11->DrawIndexed(m_IndexNum, 0, 0);
	if (m_Children.size() > 0){}
}


void MESH::Delete(){

	m_Materials->Shutdown();

	if (m_VertexBuffer != nullptr){

		m_VertexBuffer->Delete();
		delete m_VertexBuffer;
		m_VertexBuffer = nullptr;
	}
	if (m_Index != nullptr){

		m_Index->Delete();
		delete m_Index;
		m_Index = nullptr;
	}
	if (m_MyVertex != nullptr){

		delete m_MyVertex;
		m_MyVertex = nullptr;
	}
}

void MESH::AddChildren(MESH* _newChild){

	m_Children.push_back(_newChild);
}

void MESH::SetParent(MESH* _parent){

	m_Parent = _parent;
}

void MESH::SetVertex(SimpleVertex* MyVertex, int numVertex){

	m_VertexNum = numVertex;
	m_MyVertex	= MyVertex;
}

void MESH::SetIndexList(WORD* _index, int numIndex) {

	m_MyIndex	= _index;
	m_IndexNum	= numIndex;
}