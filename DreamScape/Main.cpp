/*
DX11_Hello_Triangle
Description: 
	Project to setup a DX11 rendering window, that renders a colored triangle.
	This project is meant to be used a a template for the 3D programming courses.

Implemented by:
	Diego Navarro, 2025.

Acknowledgement:
	Based on the previous work of Joakim Ståhle-Nilsson.
*/


//IMPORTING HEADER FILES --------------------------------------------------------
#include <Windows.h> //Header to access the main Windows API.
#include <iostream> //Header to access the standard input/output.
#include <d3d11.h> //Header to access the core components of DX11.
#include "WindowHelper.h" //Header to access WindowHelper methods.
#include "D3D11Helper.h" // Header to access the D3D11Helpr methods.
#include "PipelineHelper.h" // Header to access pipeline buffers, and the triangle coordinates.


//DECLARING METHODS -------------------------------------------------------------
void Render(ID3D11DeviceContext* immediateContext, ID3D11RenderTargetView* rtv,
	ID3D11DepthStencilView* dsView, D3D11_VIEWPORT& viewport, ID3D11VertexShader* vShader,
	ID3D11PixelShader* pShader, ID3D11InputLayout* inputLayout, ID3D11SamplerState*& sampler, RenderObject* obj)
{

	float clearColour[4] = { 0, 0, 0, 0 };
	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, &obj->vertexBuffer, &stride, &offset);
	immediateContext->IASetIndexBuffer(obj->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	immediateContext->VSSetShader(vShader, nullptr, 0);

	immediateContext->RSSetViewports(1, &viewport);

	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->PSSetConstantBuffers(0, 1, &obj->materialBuffer);
	immediateContext->PSSetShaderResources(0, 1, &obj->textureSRV);
	immediateContext->PSSetSamplers(0, 1, &sampler);

	immediateContext->OMSetRenderTargets(1, &rtv, dsView);

	immediateContext->DrawIndexed(obj->indexCount, 0, 0);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstnace,	_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{

	///////////Debug Console////////////////
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);
	////////////////////////////////////////

	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;
	HWND window;

	if (!SetupWindow(hInstnace, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Error: Failed to setup window" << std::endl;
		return -1;
	}

	//Intialization of DX11 interfaces.
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewport;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* inputLayout;
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* textureSRV = nullptr;
	ID3D11SamplerState* sampler;


	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Error: failed to setup D3D11" << std::endl;
		return -1;
	}

	RenderObject object;
	if (!SetupPipeline(device, object, vShader, pShader, inputLayout, texture, textureSRV, sampler))
	{
		std::cerr << "Error: failed to setup graphic pipeline" << std::endl;
		return - 1;
	}
	
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		RenderObject* obj = &object;

		Render(immediateContext, rtv, dsView, viewport, vShader, pShader, inputLayout, sampler, obj);
		swapChain->Present(0, 0);
	}

	sampler->Release();
	object.textureSRV->Release();
	object.vertexBuffer->Release();
	object.indexBuffer->Release();
	object.materialBuffer->Release();
	inputLayout->Release();
	pShader->Release();
	vShader->Release();
	dsView->Release();
	dsTexture->Release();
	rtv->Release();
	swapChain->Release();
	immediateContext->Release();
	device->Release();

	return 0;
}