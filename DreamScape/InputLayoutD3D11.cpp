#include "InputLayoutD3D11.h"



InputLayoutD3D11::~InputLayoutD3D11()
{
}
//Works so long as the vector doesnt become too large, probably wont need to but might need to fix
void InputLayoutD3D11::AddInputElement(const std::string& semanticName, DXGI_FORMAT format)
{
	semanticNames.push_back(semanticName);
	D3D11_INPUT_ELEMENT_DESC inputDesc = { semanticNames.back().c_str(), 0,format, 0,offset, D3D11_INPUT_PER_VERTEX_DATA,0};
	elements.push_back(inputDesc);
	UINT offsetAmount = GetFormatSize(format);
	offset += offsetAmount;
}

void InputLayoutD3D11::FinalizeInputLayout(ID3D11Device* device, const void* vsDataPtr, size_t vsDataSize)
{

	if (FAILED(device->CreateInputLayout(elements.data(), elements.size(), vsDataPtr, vsDataSize, inputLayout.GetAddressOf()))) {
		std::cerr << "Failed to create Input Layout" << std::endl;
	}
}

UINT InputLayoutD3D11::GetFormatSize(DXGI_FORMAT format)
{
	switch (format) {
	case DXGI_FORMAT_R32G32B32_FLOAT: return 12;
	case DXGI_FORMAT_R32G32_FLOAT: return 8;
	case DXGI_FORMAT_R32_FLOAT: return 4;
	case DXGI_FORMAT_R32G32B32A32_FLOAT: return 16;

	default: return 0;
	}
}

ID3D11InputLayout* InputLayoutD3D11::GetInputLayout() const
{
	return inputLayout.Get();
}
