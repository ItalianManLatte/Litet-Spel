#include "ShaderResourceTextureD3D11.h"

ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, UINT width, UINT height, void* textureData)
{
	Initialize(device, width, height, textureData);
}

ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, const char* pathToTextureFile)
{
	Initialize(device, pathToTextureFile);
}

ShaderResourceTextureD3D11::~ShaderResourceTextureD3D11()
{
}

void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, void* textureData) {

	D3D11_TEXTURE2D_DESC texturedesc = {};
	texturedesc.Width = width;
	texturedesc.Height = height;
	texturedesc.MipLevels = 1;
	texturedesc.ArraySize = 1;
	texturedesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	texturedesc.SampleDesc.Count = 1;
	texturedesc.SampleDesc.Quality = 0;
	texturedesc.Usage = D3D11_USAGE_IMMUTABLE;
	texturedesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texturedesc.CPUAccessFlags = 0;
	texturedesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = textureData;
	initData.SysMemPitch = width * 4;
	initData.SysMemSlicePitch = width * height * 4;

	if (FAILED(device->CreateTexture2D(&texturedesc, &initData, texture.GetAddressOf()))) {
		std::cerr << "Failed to create texture2D" << std::endl;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texturedesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(texture.Get(), &srvDesc, srv.GetAddressOf()))) {
		std::cerr << "Failed to create srv" << std::endl;
	}


}
void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, const char* pathToTextureFile) {
	
	std::string filenameStr(pathToTextureFile);
	int height = 0, width = 0, channels = 0;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* imageData = stbi_load(filenameStr.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	D3D11_TEXTURE2D_DESC texturedesc = {};
	texturedesc.Width = width;
	texturedesc.Height = height;
	texturedesc.MipLevels = 1;
	texturedesc.ArraySize = 1;
	texturedesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	texturedesc.SampleDesc.Count = 1;
	texturedesc.SampleDesc.Quality = 0;
	texturedesc.Usage = D3D11_USAGE_IMMUTABLE;
	texturedesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texturedesc.CPUAccessFlags = 0;
	texturedesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = imageData;
	initData.SysMemPitch = width * 4;
	initData.SysMemSlicePitch = width * height * 4;

	if (FAILED(device->CreateTexture2D(&texturedesc, &initData, texture.GetAddressOf()))) {
		std::cerr << "Failed to create texture2D from: " << filenameStr << std::endl;
	}

	stbi_image_free(imageData);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texturedesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(texture.Get(), &srvDesc, srv.GetAddressOf()))) {
		std::cerr << "Failed to create srv from: " << filenameStr << std::endl;
	}
}


ID3D11ShaderResourceView* ShaderResourceTextureD3D11::GetSRV() const {
	return srv.Get();
}