#include "StructuredBufferD3D11.h"

StructuredBufferD3D11::StructuredBufferD3D11(ID3D11Device* device, UINT sizeOfElement, size_t nrOfElementsInBuffer, void* bufferData, bool dynamic)
{
}

StructuredBufferD3D11::~StructuredBufferD3D11()
{
}
/*

ComPtr <ID3D11Buffer> buffer = nullptr;
	ComPtr <ID3D11ShaderResourceView> srv = nullptr;
	UINT elementSize = 0;
	size_t nrOfElements = 0;

*/

void StructuredBufferD3D11:: Initialize(ID3D11Device* device, UINT sizeOfElement, size_t nrOfElementsInBuffer, void* bufferData, bool dynamic)
{
	elementSize = sizeOfElement;
	nrOfElements = nrOfElementsInBuffer;

	D3D11_BUFFER_DESC bufferDesc = {};
	if (dynamic)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	}
	
	else
	{
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.CPUAccessFlags = 0;

	}
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeOfElement;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = bufferData;

	if (FAILED(device->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf()))) {
		std::cerr << "Failed to create structured buffer" << std::endl;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = nrOfElementsInBuffer;
			
}
void StructuredBufferD3D11::UpdateBuffer(ID3D11DeviceContext* context, void* data)
{
			
}
UINT StructuredBufferD3D11::GetElementSize() const
{
			
}
size_t StructuredBufferD3D11:: GetNrOfElements() const
{
		
}
ID3D11ShaderResourceView* StructuredBufferD3D11::GetSRV() const
{
		
}
