#include "ConstantBufferD3D11.h"

ConstantBufferD3D11::ConstantBufferD3D11(ID3D11Device* device, size_t byteSize, void* initialData)
{
	//not sure if bytesize should be = bytewidth or if it should be sizeof(simpleVertex)*byteSize
	CreateInitialData(device, byteSize, initialData);

}

ConstantBufferD3D11::~ConstantBufferD3D11()
{
}

ConstantBufferD3D11::ConstantBufferD3D11(ConstantBufferD3D11&& other) noexcept
{
}

ConstantBufferD3D11& ConstantBufferD3D11::operator=(ConstantBufferD3D11&& other) noexcept
{
	// TODO: insert return statement here
}

void ConstantBufferD3D11::CreateInitialData(ID3D11Device* device, size_t byteSize, void* initialData)
{
	D3D11_BUFFER_DESC vBufferDesc{};
	vBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vBufferDesc.ByteWidth = byteSize;
	vBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA vData{};
	vData.pSysMem = initialData;

	if (FAILED(device->CreateBuffer(&vBufferDesc, &vData, buffer.GetAddressOf()))) {
		std::cerr << "Error Could not create constant Buffer" << std::endl;
	}
}

void ConstantBufferD3D11::Initialize(ID3D11Device* device, size_t byteSize, void* initialData)
{
	CreateInitialData(device, byteSize, initialData);
}

size_t ConstantBufferD3D11::GetSize() const
{
	return bufferSize;
}

ID3D11Buffer* ConstantBufferD3D11::GetBuffer() const
{
	return buffer.Get();
}

void ConstantBufferD3D11::UpdateBuffer(ID3D11DeviceContext* context, void* data)
{
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HRESULT hr = context->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (SUCCEEDED(hr)) {
		memcpy(mapped.pData, data, bufferSize);
		context->Unmap(buffer.Get(), 0);
	}
	
}
