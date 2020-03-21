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

#ifdef D3D11
void ClaseBuffer::createVertexBuffer(int _numvertex, const aiScene* _model, SimpleVertex* _vertex, ID3D11Buffer*& _buffer, ClaseDevice* _dev) {

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

void ClaseBuffer::createIndexBuffer(int numindices, const aiScene* _model, WORD* _index, ID3D11Buffer*& _buffer, ClaseDevice* _dev) {

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
#endif // D3D11



void ClaseBuffer::OpenGLVAO(){

	//VAO

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] =
	{
	   -1.0f, -1.0f, 0.0f,
	   0.0f, -1.0f, 0.0f,
	   1.0f,  1.0f, 0.0f,
	   0.0f,  1.0f, 0.0f,
	};

	// This will identify our vertex buffer
	//GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &m_vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	unsigned int index[] =
	{
		0,1,3,
		1,2,3
	};

	//GLuint elementbuffer;// Index Buffer
	glGenBuffers(1, &m_elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &index[0], GL_STATIC_DRAW);
}

//VertexBuffer OpenGl
void ClaseBuffer::createVertexBuffer(int _numvertex, SimpleVertex* _vertex, GLuint& _buffer){

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &_buffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(SimpleVertex) * _numvertex, _vertex, GL_STATIC_DRAW);
}

//IndexBufferOpenGL
void ClaseBuffer::createIndexBuffer(int numindices, WORD* _index, GLuint& _buffer){

	//GLuint elementbuffer;// Index Buffer
	glGenBuffers(1, &_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(WORD), &_index[0], GL_STATIC_DRAW);
}
