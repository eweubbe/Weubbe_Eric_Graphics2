// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include <DirectXMath.h>

//include compiled shaders
#include "myVShader.csh"
#include "myPShader.csh"

using namespace DirectX;

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	
	//video card version of new
	ID3D11Buffer* vBuffer;
	//descirbes what a vertex looks like to directx
	ID3D11InputLayout* vLayout;
	//viewport
	D3D11_VIEWPORT myPort;
	//shader variables
	ID3D11VertexShader* vShader; //HLSL (high level shading laguage)
	ID3D11PixelShader* pShader; //HLSL

	struct Vertex
	{
		XMFLOAT4 pos;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
};

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	if (attatchPoint) // valid window?
	{
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)

			HRESULT hr;

			//set up viewport
			UINT vpWidth;
			UINT vpHeight;
			attatchPoint->GetClientWidth(vpWidth);
			attatchPoint->GetClientHeight(vpHeight);

			myPort.Width = (float)vpWidth;
			myPort.Height = (float)vpHeight;
			myPort.TopLeftX = 0;
			myPort.TopLeftY = 0;
			myPort.MinDepth = 0;
			myPort.MaxDepth = 1;

			//example triangle
			//Vertex tri[3] = // NDC (normalized device coordinates), build clockwise
			//[ // xyzw, rgba
			//	{ {0, 0.5f, 0, 1}, {1,1,1,1} },
			//	{ {0.5f, -0.5f, 0, 1}, {1,1,1,1} },
			//	{ {-0.5f, -0.5f, 0, 1}, {1,1,1,1} }
			//];
			Vertex tri[3];
			tri[0].pos.x = 0;
			tri[0].pos.y = 0.5f;
			tri[0].pos.z = 0;
			tri[0].pos.w = 1;
			tri[0].color.x = 1;
			tri[0].color.y = 1;
			tri[0].color.z = 1;
			tri[0].color.w = 1;
			tri[0].uv.x = 0;
			tri[0].uv.y = 0;

			tri[1].pos.x = 0.5f;
			tri[1].pos.y = -0.5f;
			tri[1].pos.z = 0;
			tri[1].pos.w = 1;
			tri[1].color.x = 1;
			tri[1].color.y = 1;
			tri[1].color.z = 1;
			tri[1].color.w = 1;
			tri[1].uv.x = 0;
			tri[1].uv.y = 0;

			tri[2].pos.x = -0.5f;
			tri[2].pos.y = -0.5f;
			tri[2].pos.z = 0;
			tri[2].pos.w = 1;
			tri[2].color.x = 1;
			tri[2].color.y = 1;
			tri[2].color.z = 1;
			tri[2].color.w = 1;
			tri[2].uv.x = 0;
			tri[2].uv.y = 0;

			//LOAD TRIANGLE ONTO THE VIDEO CARD////////////////////////////////////
			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData;
			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));

			//set up buffer description
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(Vertex) * 3;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			bDesc.Usage = D3D11_USAGE_DEFAULT; //would change if constantly rewriting a constant buffer


			//set up subdata
			subData.pSysMem = tri;

			//create buffer
			hr = myDevice->CreateBuffer(&bDesc, &subData, &vBuffer);
			////////////////////////////////////////////////////////////////////

			//WRITE, COMPILE AND LOAD SHADERS//////////////////////////////////
			//watch hlsl video for more on shaders
			//vertex shader
			hr = myDevice->CreateVertexShader(MyVShader, sizeof(MyVShader), nullptr, &vShader);
			hr = myDevice->CreatePixelShader(MyPShader, sizeof(MyPShader), nullptr, &pShader);
			///////////////////////////////////////////////////////////////////

			//DESCRIBE THE VERTEX TO D3D11/////////////////////////////////////

			//input element descriptor, glues c++ vertex struct to hlsl vertex struct
			D3D11_INPUT_ELEMENT_DESC ieDesc[] = 
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			//input layout (day 1 vid)
			hr = myDevice->CreateInputLayout(ieDesc, 3, MyVShader, sizeof(MyVShader), &vLayout);
			////////////////////////////////////////////////////////////////////

			hr = 0;

		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	// Release DX Objects aquired from the surface
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();

	// TODO: "Release()" more stuff here!
	vBuffer->Release();
	vLayout->Release();
	vShader->Release();
	pShader->Release();

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		// this could be changed during resolution edits, get it every frame
		ID3D11RenderTargetView *myRenderTargetView = nullptr;
		ID3D11DepthStencilView *myDepthStencilView = nullptr;
		if (G_SUCCESS(mySurface->GetRenderTarget((void**)&myRenderTargetView)))
		{
			// Grab the Z Buffer if one was requested
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
				myDepthStencilView->Release();
			}

			// Clear the screen to dark green
			const float d_green[] = { 0, 0.5f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			
			//SET UP THE PIPELINE/////////////////////////////////////////////////////////
			//output merger
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!
			//rasterizer
			myContext->RSSetViewports(1, &myPort);

			//input assembler
			myContext->IASetInputLayout(vLayout);
			ID3D11Buffer* tempVB[] = { vBuffer }; // multiple buffers would be for splitting data up, i.e. separate buffers for pos and color
			UINT strides[] = {sizeof(Vertex)}; //distance between 2 vertecies
			UINT offsets[] = {0}; //where to start from in array
			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //what do we want it to draw? line, triangle, etc.

			//vertex shader stage
			myContext->VSSetShader(vShader, 0, 0);

			//pixel shader stage
			myContext->PSSetShader(pShader, 0, 0);

			//Draw (nothing actually happens until draw is called)
			myContext->Draw(3, 0);

			/////////////////////////////////////////////////////////////////////////////

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}