#include "..\Encabezados\ClaseVertexBuffer.h"

void ClaseVertexBuffer::Init(SUBRESOURCE_DATA _data, BufferDescriptor _buffer){

	m_Buffer.Init(_buffer);

	ZeroMemory(&m_SubDataD3D11, sizeof(m_SubDataD3D11));
	m_SubDataD3D11.pSysMem = _data.My_pSysMem;
}

void ClaseVertexBuffer::Update(){
}

void ClaseVertexBuffer::Delete(){
}

void ClaseVertexBuffer::Render(){
}
