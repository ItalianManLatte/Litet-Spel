//Prepocessor command to ensure that this file is included once per compilation.
#pragma once

//IMPORTING HEADER FILES --------------------------------------------------------
#include <array> //Header that provides a fixe size stack-allocated array with STL container semantincs (i.e. typical array functionality expected in C++).
#include <d3d11.h> //Header to access the core components of DX11.
#include <DirectXMath.h>
#include <string>

//DECLARING METHODS -------------------------------------------------------------
struct SimpleVertex
{
	float pos[3];
	float clr[3];
	float nrm[3];
	float uv[2];

	SimpleVertex(const std::array<float, 3> position, const std::array<float, 3> colour, const std::array<float,3> normal, const std::array<float, 2> texCoord)
	{
		for (int i = 0; i < 3; i++)
		{
			pos[i] = position[i];
			clr[i] = colour[i];
			nrm[i] = normal[i];
		}
		uv[0] = texCoord[0];
		uv[1] = texCoord[1];
	}
};

struct MaterialGPU 
{
	DirectX::XMFLOAT3 Ka;
	float matPad0;
	DirectX::XMFLOAT3 Kd;
	float matPad1;
	DirectX::XMFLOAT3 Ks;
	float matPad2;
	DirectX::XMFLOAT3 Ke;
	float matPad3;
	
	float Ns;
	float Ni;
	float d;
	int illum;
};

struct RenderObject 
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	UINT vertexCount = 0;
	UINT indexCount = 0;

	ID3D11ShaderResourceView* textureSRV = nullptr;

	MaterialGPU material;
	ID3D11Buffer* materialBuffer = nullptr;
};

bool LoadTextureFromPNG(ID3D11Device* device, const std::string& filename, ID3D11ShaderResourceView*& outSRV);

bool SetupPipeline(ID3D11Device* device, RenderObject& object, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, 
	ID3D11InputLayout*& inputLayout, ID3D11Texture2D* textureBuffer, ID3D11ShaderResourceView* textureSRV, ID3D11SamplerState*& sampler);