#include "IndexBufferD3D11.h"



IndexBufferD3D11::IndexBufferD3D11(ID3D11Device* device, size_t nrOfIndicesInBuffer, uint32_t* indexData)
{
	InitializeData(device, nrOfIndicesInBuffer, indexData);

}

IndexBufferD3D11::~IndexBufferD3D11()
{
}

void IndexBufferD3D11::InitializeData(ID3D11Device* device, size_t nrOfIndicesInBuffer, uint32_t* indexData) {
	
	D3D11_BUFFER_DESC ibData = {};
	ibData.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * nrOfIndicesInBuffer);
	ibData.Usage = D3D11_USAGE_IMMUTABLE;
	ibData.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibData.CPUAccessFlags = 0;
	ibData.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexSubData = {};
	indexSubData.pSysMem = indexData;
	indexSubData.SysMemPitch = 0;
	indexSubData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&ibData, &indexSubData, buffer.GetAddressOf()))) {
		std::cerr << "Could not create index buffer" << std::endl;
	}
}

void IndexBufferD3D11::Initialize(ID3D11Device* device, size_t nrOfIndicesInBuffer, uint32_t* indexData) {
	InitializeData(device, nrOfIndicesInBuffer, indexData);
}
size_t IndexBufferD3D11::GetNrOfIndices() const {
	return nrOfIndices;
}
