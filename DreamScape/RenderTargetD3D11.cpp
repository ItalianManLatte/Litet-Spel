#include "RenderTargetD3D11.h"

/*
ID3D11Texture2D* texture = nullptr; //backbuffer
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
*/


RenderTargetD3D11::~RenderTargetD3D11()
{
}
void RenderTargetD3D11::Initialize(ID3D11Device* device, UINT width, UINT height,
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, bool hasSRV = false) {

	texture.Reset();
	rtv.Reset();
	srv.Reset();

	//desc and creae texture
	D3D11_TEXTURE2D_DESC rtvTexDesc = {};
	
	rtvTexDesc.Width = width;
	rtvTexDesc.Height = height;
	rtvTexDesc.Format = format;
	rtvTexDesc.MipLevels = 1;
	rtvTexDesc.ArraySize = 1;
	rtvTexDesc.SampleDesc.Count = 1;
	rtvTexDesc.SampleDesc.Quality = 0;
	rtvTexDesc.Usage = D3D11_USAGE_DEFAULT;
	rtvTexDesc.BindFlags = 0;
	rtvTexDesc.MiscFlags = 0;
	if (hasSRV)
		rtvTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	else
		rtvTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET;


	if (FAILED(device->CreateTexture2D(&rtvTexDesc, nullptr, texture.GetAddressOf()))) {
		std::cerr << "Failed to create texture2D backBuffer" << std::endl;
	}

	if (FAILED(device->CreateRenderTargetView(texture.Get(), nullptr, rtv.GetAddressOf()))) {
		std::cerr << "Failed to create Render Target View" << std::endl;
	}
	if (hasSRV) {
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		
		if (FAILED(device->CreateShaderResourceView(texture.Get(), &srvDesc, srv.GetAddressOf()))) {
			std::cerr << "Failed to create shader resource view for RTV" << std::endl;
		}
	}
}

ID3D11RenderTargetView* RenderTargetD3D11::GetRTV() const {
	return rtv.Get();
}
ID3D11ShaderResourceView* RenderTargetD3D11::GetSRV() const {
	return srv.Get();
}