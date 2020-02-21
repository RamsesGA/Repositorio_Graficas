#include "..\Encabezados\ClaseSampleState.h"

void ClaseSampleState::Init(SampleStateDesc _sampleDesc){

	m_SampleDesc = _sampleDesc;

	ZeroMemory(&sampDescD3D11, sizeof(sampDescD3D11));

	sampDescD3D11.Filter			= (D3D11_FILTER)m_SampleDesc.Filter;
	sampDescD3D11.AddressU			= (D3D11_TEXTURE_ADDRESS_MODE)m_SampleDesc.AddressU;
	sampDescD3D11.AddressV			= (D3D11_TEXTURE_ADDRESS_MODE)m_SampleDesc.AddressV;
	sampDescD3D11.AddressW			= (D3D11_TEXTURE_ADDRESS_MODE)m_SampleDesc.AddressW;
	sampDescD3D11.ComparisonFunc	= (D3D11_COMPARISON_FUNC)m_SampleDesc.ComparisonFunc;
	sampDescD3D11.MinLOD			= m_SampleDesc.MinLOD;
	sampDescD3D11.MaxLOD			= m_SampleDesc.MaxLOD;
}	

	