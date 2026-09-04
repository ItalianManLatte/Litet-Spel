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

	//desc and creae texture
	D3D11_TEXTURE2D_DESC rtvTexDesc = {};
	
	rtvTexDesc.Width = width;
	rtvTexDesc.Height = height;
	rtvTexDesc.Format = format;
	rtvTexDesc.MipLevels = 1;
	rtvTexDesc.ArraySize = 1;
	rtvTexDesc.SampleDesc.Count = 1;
	rtvTexDesc.SampleDesc.Quality = 1;
	rtvTexDesc.Usage = D3D11_USAGE_DEFAULT;
	if (hasSRV)
		rtvTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	else
		rtvTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	rtvTexDesc.BindFlags = 0;
	rtvTexDesc.MiscFlags = 0;
	if(FAILED(device->CreateTexture2D(&rtvTexDesc, nullptr, )))


	

}
ID3D11RenderTargetView* RenderTargetD3D11::GetRTV() const {

}
ID3D11ShaderResourceView* RenderTargetD3D11::GetSRV() const {

}