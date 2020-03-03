#include "..\Encabezados\ClaseBuffer.h"

void ClaseBuffer::Init(BufferDescriptor _buff){

	m_DescBuff = _buff;
#ifdef D3D11
	ZeroMemory(&m_BufferDescD3D11, sizeof(m_BufferDescD3D11));

	m_BufferDescD3D11.Usage = (D3D11_USAGE)m_DescBuff.Usage;
	m_BufferDescD3D11.ByteWidth = m_DescBuff.ByteWidth;
	m_BufferDescD3D11.BindFlags = m_DescBuff.BindFlags;
	m_BufferDescD3D11.CPUAccessFlags = m_DescBuff.CPUAccessFlags;
#endif
}

void ClaseBuffer::Update(){
}

void ClaseBuffer::Delete(){
}

void ClaseBuffer::Render(){
}

void ClaseBuffer::createVertexBuffer(int _numvertex, const aiScene* _model, SimpleVertex* _vertex, ID3D11Buffer*& _buffer, ClaseDevice* _dev){

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * _numvertex;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = _vertex;
	_dev->g_pd3dDeviceD3D11->CreateBuffer(&bd, &InitData, &_buffer);
}

void ClaseBuffer::createIndexBuffer(int numindices, const aiScene* _model, WORD* _index, ID3D11Buffer*& _buffer, ClaseDevice* _dev){

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData; 
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * numindices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = _index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = _dev->g_pd3dDeviceD3D11->CreateBuffer(&bd, &InitData, &_buffer);
	if (FAILED(hr))
		return;
}

/*//Set
void ClaseBuffer::SetByteW(UINT _byte){

	m_DescBuff.ByteWidth = _byte;
}

void ClaseBuffer::SetUsage(D3D11_USAGE _usage){

	m_DescBuff.Usage = _usage;
}

void ClaseBuffer::SetBindF(UINT _bFlags){

	m_DescBuff.BindFlags = _bFlags;
}

void ClaseBuffer::SetCPUAccess(UINT _cpu){

	m_DescBuff.CPUAccessFlags = _cpu;
}

void ClaseBuffer::SetMiscF(UINT _misc) {

	m_DescBuff.MiscFlags = _misc;
}

void ClaseBuffer::SetStructB(UINT _struct) {

	m_DescBuff.StructureByteStride = _struct;
}*/