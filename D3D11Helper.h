#pragma once

//IMPORTING HEADER FILES --------------------------------------------------------
#include <Windows.h> //Header to access the main Windows API.
#include <d3d11.h> //Header to access the core component and interfaces of the DirectX 11 API.
#include <iostream> //Header to access the standard input/output.

//DECLARING METHODS -------------------------------------------------------------

bool SetupD3D11(
	UINT width,
	UINT height,
	HWND window,
	ID3D11Device*& device,
	ID3D11DeviceContext*& immediateContext,
	IDXGISwapChain*& swapChain,
	ID3D11RenderTargetView*& rtv,
	ID3D11Texture2D*& dsTexture,
	ID3D11DepthStencilView*& dsView,
	D3D11_VIEWPORT& viewport
);