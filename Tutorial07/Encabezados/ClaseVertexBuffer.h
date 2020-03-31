#pragma once
#include "ClaseBuffer.h"

///
/// Class with the Vertex Buffer abstraction
///
class ClaseVertexBuffer {

	public:

		///
		/// Builder and destroyer
		///
		ClaseVertexBuffer() {};
		~ClaseVertexBuffer() {};

		///
		/// Members
		///
		ClaseBuffer m_Buffer;

		///
		/// Main methods for the class
		///
		void
		Init(SUBRESOURCE_DATA _data, BufferDescriptor _buffer);

		void
		Update();

		void
		Delete();

		void
		Render();

		///
		/// DIRECTX members
		///
#ifdef D3D11
		D3D11_SUBRESOURCE_DATA m_SubDataD3D11;
#endif // D3D11
};