#pragma once
#include "Defines.h"

struct BufferDescriptor {

	UINT ByteWidth;
	USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;
};

class ClaseBuffer {

	public:
		
		BufferDescriptor m_DescBuff;

		ClaseBuffer() {

#ifdef D3D11
			m_BufferD3D11 = NULL;
#endif // D3D11
		};
		~ClaseBuffer() {};

		/*//Abstractions
		void 
		SetByteW(UINT _byte);
		UINT
		GetByteW() { return m_DescBuff.ByteWidth; };

		void 
		SetUsage(D3D11_USAGE _usage);
		D3D11_USAGE
		GetUsage() { return m_DescBuff.Usage; };

		void 
		SetBindF(UINT _bFlags);
		UINT
		GetBindF() { return m_DescBuff.BindFlags; };

		void 
		SetCPUAccess(UINT _cpu);
		UINT
		GetCPUAccess() { return m_DescBuff.CPUAccessFlags; };

		void 
		SetMiscF(UINT _misc);
		UINT
		GetMiscF() { return m_DescBuff.MiscFlags; };

		void 
		SetStructB(UINT _struct);
		UINT
		GetStructB() { return m_DescBuff.StructureByteStride; };*/

		//Functions
		void
		Init(BufferDescriptor _buff);
		void
		Update();
		void
		Delete();
		void
		Render();

#ifdef D3D11
		ID3D11Buffer* m_BufferD3D11;
		D3D11_BUFFER_DESC m_BufferDescD3D11;
#endif
};