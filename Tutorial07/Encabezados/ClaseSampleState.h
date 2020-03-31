#pragma once
#include "Defines.h"

///
/// Sample State Descriptor
///
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

///
/// Class with the abstraction of the Sample State
///
class ClaseSampleState {

	public:

		///
		/// Builders and destroyers
		///
		ClaseSampleState() {};
		~ClaseSampleState() {};

		///
		/// Members
		///
		SampleStateDesc m_SampleDesc;

		///
		/// Methods
		///
		void
		Init(SampleStateDesc _sampleDesc);

		void
		Update();

		void
		Destroy();

		void
		Render();

		///
		/// DIRECTX members
		///
#ifdef D3D11
		ID3D11SamplerState*			g_pSamplerLinearD3D11;
		D3D11_SAMPLER_DESC			sampDescD3D11;
#endif // D3D11
};