#include "VertexBufferD3D11.h"

VertexBufferD3D11::VertexBufferD3D11(ID3D11Device* device, UINT sizeOfVertex, UINT nrOfVerticesInBuffer, void* vertexData)
{
	InitializeData(device, sizeOfVertex, nrOfVerticesInBuffer, vertexData);
}

VertexBufferD3D11::~VertexBufferD3D11()
{

}
void VertexBufferD3D11::Initialize(ID3D11Device* device, UINT sizeOfVertex, UINT nrOfVerticesInBuffer, void* vertexData)
{
	InitializeData(device, sizeOfVertex, nrOfVerticesInBuffer, vertexData);
}

void VertexBufferD3D11::InitializeData(ID3D11Device* device, UINT sizeOfVertex, UINT nrOfVerticesInBuffer, void* vertexData)
{
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.ByteWidth = sizeOfVertex * nrOfVerticesInBuffer;
	vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vData = {};
	vData.pSysMem = vertexData;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vbDesc, &vData, buffer.GetAddressOf()))) {
		std::cerr << "Failed to create Vertex Buffer" << std::endl;
	}
}

UINT VertexBufferD3D11::GetNrOfVertices() const {
	return nrOfVertices;
}

UINT VertexBufferD3D11::GetVertexSize() const {
	return vertexSize;
}

ID3D11Buffer* VertexBufferD3D11::GetBuffer() const {
	return buffer.Get();
}
