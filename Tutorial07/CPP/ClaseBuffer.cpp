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