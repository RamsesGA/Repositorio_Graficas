#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
struct VertexBufferDesc {

};

class ClaseVertexBuffer {

	public:
		ClaseVertexBuffer() {};
		~ClaseVertexBuffer() {};

		void
		Init();

		void
		Update();

		void
		Delete();

		void
		Render();

#ifdef D3D11
#endif // D3D11
};