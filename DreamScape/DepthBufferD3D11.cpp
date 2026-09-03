#include "DepthBufferD3D11.h"

DepthBufferD3D11::DepthBufferD3D11(ID3D11Device* device, UINT width, UINT height, bool hasSRV)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()))) {
		std::cerr << "Failed to create Depth Buffer texture" << std::endl;
	}
	
	//HRESULT hr;
	for (ComPtr <ID3D11DepthStencilView> dsv : depthStencilViews) {
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Format = textureDesc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		if (FAILED(device->CreateDepthStencilView(texture.Get(), &dsvDesc, dsv.GetAddressOf()))) {
			std::cerr << "Failed to create Depth Stencil View" << std::endl;
		}
	}

	if (hasSRV) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		if (FAILED(device->CreateShaderResourceView(texture.Get(), &srvDesc, srv.GetAddressOf()))) {
			std::cerr << "Failed to create Shader Resource View for Depth Stencil" << std::endl;
		}

	}
	
}

DepthBufferD3D11::~DepthBufferD3D11()
{
}

void DepthBufferD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, bool hasSRV, UINT arraySize)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf()))) {
		std::cerr << "Failed to create Depth Buffer texture" << std::endl;
	}

	//HRESULT hr;
	for (ComPtr <ID3D11DepthStencilView> dsv : depthStencilViews) {
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Format = textureDesc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		if (FAILED(device->CreateDepthStencilView(texture.Get(), &dsvDesc, dsv.GetAddressOf()))) {
			std::cerr << "Failed to create Depth Stencil View" << std::endl;
		}
	}

	if (hasSRV) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		if (FAILED(device->CreateShaderResourceView(texture.Get(), &srvDesc, srv.GetAddressOf()))) {
			std::cerr << "Failed to create Shader Resource View for Depth Stencil" << std::endl;
		}

	}


}

ID3D11DepthStencilView* DepthBufferD3D11::GetDSV(UINT arrayIndex) const
{
	return depthStencilViews[arrayIndex].Get();
}

ID3D11ShaderResourceView* DepthBufferD3D11::GetSRV() const
{
	return srv.Get();
}
