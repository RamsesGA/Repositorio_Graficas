#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

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

		ClaseBuffer() {};
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
		Init();

		void
		Update();

		void
		Delete();

		void
		Render();
};