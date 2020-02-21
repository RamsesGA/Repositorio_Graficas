#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

struct Texture2Desc {

    UINT Width;
    UINT Height;
    UINT MipLevels;
    UINT ArraySize;

    FORMAT Format;
    SAMPLE_DESC SampleDesc;
    USAGE Usage;

    UINT BindFlags;
    UINT CPUAccessFlags;
    UINT MiscFlags;
};

class ClaseTextura2D {

	public:
		ClaseTextura2D() {};
		~ClaseTextura2D() {};

		void
		Init();

#ifdef D3D11
#endif // D3D11
};