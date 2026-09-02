//Prepocessor command to ensure that this file is included once per compilation.
#pragma once

//IMPORTING HEADER FILES --------------------------------------------------------
#include <array> //Header that provides a fixe size stack-allocated array with STL container semantincs (i.e. typical array functionality expected in C++).
#include <d3d11.h> //Header to access the core components of DX11.

//DECLARING METHODS -------------------------------------------------------------
struct SimpleVertex
{
	float pos[3];
	float clr[3];

	SimpleVertex(const std::array<float, 3> position, const std::array<float, 3> colour)
	{
		for (int i = 0; i < 3; i++)
		{
			pos[i] = position[i];
			clr[i] = colour[i];
		}
	}
};

bool SetupPipeline(ID3D11Device* device, ID3D11Buffer*& vertexbuffer, ID3D11VertexShader*& vShader, ID3D11PixelShader*& pShader, ID3D11InputLayout*& inputLayout);
