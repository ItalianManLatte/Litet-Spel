#include "SamplerD3D11.h"
#include <iostream>

SamplerD3D11::SamplerD3D11(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE addressMode, std::optional<std::array<float, 4>> borderColour)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;
	std::copy(borderColour->begin(), borderColour->end(), samplerDesc.BorderColor);

	if (FAILED(device->CreateSamplerState(&samplerDesc, &this->sampler)))
	{
		std::cerr << "Failed to create sampler state." << std::endl;
	}
}

SamplerD3D11::~SamplerD3D11()
{
	this->sampler->Release();
}

void SamplerD3D11::Initialize(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE addressMode, std::optional<std::array<float, 4>> borderColour)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;
	std::copy(borderColour->begin(), borderColour->end(), samplerDesc.BorderColor);

	if (FAILED(device->CreateSamplerState(&samplerDesc, &this->sampler)))
	{
		std::cerr << "Failed to create sampler state." << std::endl;
	}
}

ID3D11SamplerState* SamplerD3D11::GetSamplerState() const
{
	return this->sampler;
}
