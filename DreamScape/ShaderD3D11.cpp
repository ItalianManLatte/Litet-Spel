#include "ShaderD3D11.h"
#include <iostream>
#include <fstream>

ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize) :
	type(shaderType), shaderByteData(dataPtr), shaderByteSize(dataSize)
{
	switch (type)
	{
	case ShaderType::VERTEX_SHADER:
		if (FAILED(device->CreateVertexShader(dataPtr, dataSize, nullptr, &shader.vertex)))
		{
			std::cerr << "Failed to create vertex shader." << std::endl;
		}
		break;

	case ShaderType::HULL_SHADER:
		if (FAILED(device->CreateHullShader(dataPtr, dataSize, nullptr, &shader.hull)))
		{
			std::cerr << "Failed to create hull shader." << std::endl;
		}
		break;

	case ShaderType::DOMAIN_SHADER:
		if (FAILED(device->CreateDomainShader(dataPtr, dataSize, nullptr, &shader.domain)))
		{
			std::cerr << "Failed to create domain shader." << std::endl;
		}
		break;

	case ShaderType::GEOMETRY_SHADER:
		if (FAILED(device->CreateGeometryShader(dataPtr, dataSize, nullptr, &shader.geometry)))
		{
			std::cerr << "Failed to create geometry shader." << std::endl;
		}
		break;

	case ShaderType::PIXEL_SHADER:
		if (FAILED(device->CreatePixelShader(dataPtr, dataSize, nullptr, &shader.pixel)))
		{
			std::cerr << "Failed to create pixel shader." << std::endl;
		}
		break;

	case ShaderType::COMPUTE_SHADER:
		if (FAILED(device->CreateComputeShader(dataPtr, dataSize, nullptr, &shader.compute)))
		{
			std::cerr << "Failed to create compute shader." << std::endl;
		}
		break;
	}
}

ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const char* csoPath) :
	type(shaderType)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open(csoPath, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Failed to open shader file." << std::endl;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign(std::istreambuf_iterator<char>(reader), std::istreambuf_iterator<char>());

	this->Initialize(device, shaderType, shaderData.c_str(), shaderData.length());
}

ShaderD3D11::~ShaderD3D11()
{
	switch (this->type)
	{
	case ShaderType::VERTEX_SHADER:
		this->shader.vertex->Release();
		break;

	case ShaderType::HULL_SHADER:
		this->shader.hull->Release();
		break;

	case ShaderType::DOMAIN_SHADER:
		this->shader.domain->Release();
		break;

	case ShaderType::GEOMETRY_SHADER:
		this->shader.geometry->Release();
		break;

	case ShaderType::PIXEL_SHADER:
		this->shader.pixel->Release();
		break;

	case ShaderType::COMPUTE_SHADER:
		this->shader.compute->Release();
		break;
	}
}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize)
{
	this->type = shaderType;
	this->shaderByteData = dataPtr;
	this->shaderByteSize = dataSize;

	switch (type)
	{
	case ShaderType::VERTEX_SHADER:
		if (FAILED(device->CreateVertexShader(dataPtr, dataSize, nullptr, &shader.vertex)))
		{
			std::cerr << "Failed to create vertex shader." << std::endl;
		}
		break;

	case ShaderType::HULL_SHADER:
		if (FAILED(device->CreateHullShader(dataPtr, dataSize, nullptr, &shader.hull)))
		{
			std::cerr << "Failed to create hull shader." << std::endl;
		}
		break;

	case ShaderType::DOMAIN_SHADER:
		if (FAILED(device->CreateDomainShader(dataPtr, dataSize, nullptr, &shader.domain)))
		{
			std::cerr << "Failed to create domain shader." << std::endl;
		}
		break;

	case ShaderType::GEOMETRY_SHADER:
		if (FAILED(device->CreateGeometryShader(dataPtr, dataSize, nullptr, &shader.geometry)))
		{
			std::cerr << "Failed to create geometry shader." << std::endl;
		}
		break;

	case ShaderType::PIXEL_SHADER:
		if (FAILED(device->CreatePixelShader(dataPtr, dataSize, nullptr, &shader.pixel)))
		{
			std::cerr << "Failed to create pixel shader." << std::endl;
		}
		break;

	case ShaderType::COMPUTE_SHADER:
		if (FAILED(device->CreateComputeShader(dataPtr, dataSize, nullptr, &shader.compute)))
		{
			std::cerr << "Failed to create compute shader." << std::endl;
		}
		break;
	}
}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	this->type = shaderType;

	std::string shaderData;
	std::ifstream reader;

	reader.open(csoPath, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Failed to open shader file." << std::endl;
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign(std::istreambuf_iterator<char>(reader), std::istreambuf_iterator<char>());

	this->Initialize(device, shaderType, shaderData.c_str(), shaderData.length());
}

const void* ShaderD3D11::GetShaderByteData() const
{
	return this->shaderByteData;
}

size_t ShaderD3D11::GetShaderByteSize() const
{
	return this->shaderByteSize;
}

void ShaderD3D11::BindShader(ID3D11DeviceContext* context) const
{
	switch (this->type)
	{
	case ShaderType::VERTEX_SHADER:
		context->VSSetShader(this->shader.vertex, nullptr, 0);
		break;

	case ShaderType::HULL_SHADER:
		context->HSSetShader(this->shader.hull, nullptr, 0);
		break;

	case ShaderType::DOMAIN_SHADER:
		context->DSSetShader(this->shader.domain, nullptr, 0);
		break;

	case ShaderType::GEOMETRY_SHADER:
		context->GSSetShader(this->shader.geometry, nullptr, 0);
		break;

	case ShaderType::PIXEL_SHADER:
		context->PSSetShader(this->shader.pixel, nullptr, 0);
		break;

	case ShaderType::COMPUTE_SHADER:
		context->CSSetShader(this->shader.compute, nullptr, 0);
		break;
	}
}
