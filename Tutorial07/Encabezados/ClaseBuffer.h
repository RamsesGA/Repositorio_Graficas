#pragma once
#include "Defines.h"
#include "ClaseDevice.h"
#include "../assimp/scene.h"

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

		ClaseBuffer() 
		{

#ifdef D3D11
			m_BufferD3D11 = NULL;
#endif // D3D11

		};
		~ClaseBuffer() {};

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

		static void 
		createVertexBuffer(int _numvertex, const aiScene* _model, SimpleVertex* _vertex, ID3D11Buffer*& _buffer, ClaseDevice* _dev);
		static void 
		createIndexBuffer(int numindices, const aiScene* _model, WORD* _index, ID3D11Buffer*& _buffer, ClaseDevice* _dev);
#endif

		GLuint m_ID;// Index Buffer
		GLuint m_vertexbuffer;
		GLuint m_elementbuffer;

		void 
		OpenGLVAO();

		static void
		createVertexBuffer(int _numvertex, SimpleVertex* _vertex, GLuint& _buffer);
		static void
		createIndexBuffer(int numindices, WORD* _index, GLuint& _buffer);
};