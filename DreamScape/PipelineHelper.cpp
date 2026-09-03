//IMPORTING HEADER FILES --------------------------------------------------------
#include "PipelineHelper.h"
#include <string> //Header to access the standar string class libraries.
#include <fstream> //Header to acess the standard library for reading/writing external files.
#include <iostream> //Header to access the standard input/output.
#include <vector>
#include <DirectXMath.h>

#include "tiny_obj_loader.h"
#include "stb_image.h"

//DECLARING METHODS -------------------------------------------------------------
bool LoadShaders(ID3D11Device* device, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, std::string& vShaderByteCode)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Error: could not open vertex shader file" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vShader)))
	{
		std::cerr << "Error: failed to create the vertex shader" << std::endl;
		return false;
	}

	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	reader.open("PixelShader.cso", std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Error: could not open thje pixel shader file" << std::endl;
		return false;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pShader)))
	{
		std::cerr << "Error: failed to create pixel shader" << std::endl;
		return false;
	}

	return true;
}

bool CreateInputLayout(ID3D11Device* device, ID3D11InputLayout*& inputLayout, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[4] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 4, vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);

	return !FAILED(hr);
}

bool LoadOBJIndexed(
	const std::string& filename,
	std::vector<SimpleVertex>& outVertices,
	std::vector<UINT>& outIndices,
	std::string& outTexture,
	MaterialGPU& outMaterial)
{
	tinyobj::ObjReader reader;
	tinyobj::ObjReaderConfig config;
	config.triangulate = true;
	config.mtl_search_path = "./";

	if (!reader.ParseFromFile(filename, config))
	{
		return false;
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();


	for (const auto& shape : shapes)
	{
		for (const auto& idx : shape.mesh.indices)
		{
			std::array<float, 3> pos = {
				attrib.vertices[3 * idx.vertex_index + 0],
				attrib.vertices[3 * idx.vertex_index + 1],
				attrib.vertices[3 * idx.vertex_index + 2]
			};

			std::array<float, 3> nrm = { 0, 0, 0 };
			if (idx.normal_index >= 0)
			{
				nrm = {
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				};
			}

			std::array<float, 2> uv = { 0, 0 };
			if (idx.texcoord_index >= 0)
			{
				uv = {
					attrib.texcoords[2 * idx.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
				};
			}

			std::array<float, 3> clr = { 1.0f, 1.0f, 1.0f };

			outVertices.emplace_back(pos, clr, nrm, uv);
			outIndices.push_back((UINT)outIndices.size());
		}
	}

	if (!materials.empty())
	{
		const auto& mat = materials[0];

		outMaterial.Ka = { mat.ambient[0], mat.ambient[1], mat.ambient[2] };
		outMaterial.Kd = { mat.diffuse[0], mat.diffuse[1], mat.diffuse[2] };
		outMaterial.Ks = { mat.specular[0], mat.specular[1], mat.specular[2] };
		outMaterial.Ke = { mat.emission[0], mat.emission[1], mat.emission[2] };

		outMaterial.Ni = mat.ior;
		outMaterial.d = mat.dissolve;
		outMaterial.illum = mat.illum;

		outMaterial.Ns = (mat.shininess > 0.0f) ? mat.shininess : 32.0f;

		if (!mat.diffuse_texname.empty())
		{
			outTexture = mat.diffuse_texname;
		}
	}

	return true;
}

bool CreateBuffersFromOBJ(
	ID3D11Device* device,
	RenderObject& obj,
	const std::string& filename)
{
	std::vector<SimpleVertex> vertices;
	std::vector<UINT> indices;
	std::string diffuseTex;
	MaterialGPU material{};

	if (!LoadOBJIndexed(filename, vertices, indices, diffuseTex, material))
		return false;

	obj.vertexCount = (UINT)vertices.size();
	obj.indexCount = (UINT)indices.size();
	obj.material = material;

	D3D11_BUFFER_DESC vbd{};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SimpleVertex) * obj.vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vData{};
	vData.pSysMem = vertices.data();

	if (FAILED(device->CreateBuffer(&vbd, &vData, &obj.vertexBuffer)))
	{
		std::cerr << "Error creating object vertex buffer!" << std::endl;
		return false;
	}

	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * obj.indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA iData{};
	iData.pSysMem = indices.data();

	if (FAILED(device->CreateBuffer(&ibd, &iData, &obj.indexBuffer)))
	{
		std::cerr << "Error creating object index buffer!" << std::endl;
		return false;
	}

	if (!diffuseTex.empty())
	{
		LoadTextureFromPNG(device, diffuseTex, obj.textureSRV);
	}

	D3D11_BUFFER_DESC mbd{};
	mbd.Usage = D3D11_USAGE_DEFAULT;
	mbd.ByteWidth = sizeof(MaterialGPU);
	mbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA mData{};
	mData.pSysMem = &obj.material;

	if (FAILED(device->CreateBuffer(&mbd, &mData, &obj.materialBuffer)))
	{
		std::cerr << "Error creating material buffer!" << std::endl;
		return false;
	}

	return true;
}

bool LoadTextureFromPNG(ID3D11Device* device, const std::string& filename, ID3D11ShaderResourceView*& OutSRV)
{
	int width, height, channels;
	stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, 4);
	if (!pixels) { return false; }

	D3D11_TEXTURE2D_DESC tDesc{};
	tDesc.Width = width;
	tDesc.Height = height;
	tDesc.MipLevels = 1;
	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.SampleDesc.Count = 1;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = pixels;
	data.SysMemPitch = width * 4;

	ID3D11Texture2D* texture = nullptr;
	if (FAILED(device->CreateTexture2D(&tDesc, &data, &texture))) {
		stbi_image_free(pixels);
		return false;
	}

	HRESULT hr = device->CreateShaderResourceView(texture, nullptr, &OutSRV);
	texture->Release();
	stbi_image_free(pixels);

	return !FAILED(hr);
}

bool CreateTextureSRVAndSampler(ID3D11Device* device, ID3D11Texture2D* texture,
	ID3D11ShaderResourceView*& srv, ID3D11SamplerState*& sampler)
{

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	HRESULT hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);
	if (FAILED(hr))
		return false;

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&sampDesc, &sampler);
	return !FAILED(hr);
}

bool SetupPipeline(ID3D11Device* device, RenderObject& object, ID3D11VertexShader*& vShader,
	ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout,ID3D11Texture2D* textureBuffer, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState*& sampler)
{
	std::string vShaderByteCode;
	if (!LoadShaders(device, vShader, pShader, vShaderByteCode))
	{
		std::cerr << "Error loading shaders!" << std::endl;
		return false;
	}

	if (!CreateInputLayout(device, inputLayout, vShaderByteCode))
	{
		std::cerr << "Error creating input layout!" << std::endl;
		return false;
	}

	object.position = { 0.0f, 0.0f,0.0f };
	object.rotation = { 0.0f, 0.0f,0.0f };
	object.scale = { 1.0f, 1.0f, 1.0f };
	
	if (!CreateBuffersFromOBJ(device, object, "spot.obj")) {
		std::cerr << "Failed to load apple.obj" << std::endl;
		return false;
	}

	if (!CreateTextureSRVAndSampler(device, textureBuffer, textureSRV, sampler)) {
		std::cerr << "Error createing SRV or Sampler";
		return false;
	}
	
	return true;
}