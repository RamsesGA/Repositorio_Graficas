#pragma once
#include "Defines.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

struct SampleStateDesc {

	SAMPLER_DESC			sampDesc;
	FILTER					Filter;
	TEXTURE_ADDRESS_MODE	AddressU;
	TEXTURE_ADDRESS_MODE	AddressV;
	TEXTURE_ADDRESS_MODE	AddressW;
	COMPARISON_FUNC			ComparisonFunc;

	FLOAT MinLOD;
	FLOAT MaxLOD;
};

class ClaseSampleState {

	public:
		ClaseSampleState() {};
		~ClaseSampleState() {};

		SampleStateDesc m_SampleDesc;

		void
		Init(SampleStateDesc _sampleDesc);

		void
		Update();

		void
		Destroy();

		void
		Render();

#ifdef D3D11
		ID3D11SamplerState*			g_pSamplerLinearD3D11;
		D3D11_SAMPLER_DESC			sampDescD3D11;
#endif // D3D11
};