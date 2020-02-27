#pragma once
#include "ClaseBuffer.h"

class ClaseVertexBuffer {

	public:
		ClaseVertexBuffer() {};
		~ClaseVertexBuffer() {};

		ClaseBuffer m_Buffer;

		void
		Init(SUBRESOURCE_DATA _data, BufferDescriptor _buffer);

		void
		Update();

		void
		Delete();

		void
		Render();

#ifdef D3D11
		D3D11_SUBRESOURCE_DATA m_SubDataD3D11;
#endif // D3D11
};