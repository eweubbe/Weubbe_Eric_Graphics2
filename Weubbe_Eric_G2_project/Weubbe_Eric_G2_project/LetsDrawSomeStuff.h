// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include"XTime.h"
#include<iostream>
#include<fstream>
#include"DynArray.h"

//include compiled shaders
#include "myVShader.csh"
#include "myPShader.csh"

using namespace DirectX;
using namespace std;

//funtime random color 
#define RAND_COLOR XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), 1.0f);

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
	ID3D11Buffer* vBuffer = nullptr;
	ID3D11Buffer* iBuffer = nullptr;
	ID3D11Buffer* cBuffer = nullptr;
	//descirbes what a vertex looks like to directx
	ID3D11InputLayout* vLayout = nullptr;
	//viewport
	D3D11_VIEWPORT myPort;
	//shader variables
	ID3D11VertexShader* vShader = nullptr; //HLSL (high level shading laguage)
	ID3D11PixelShader* pShader = nullptr; //HLSL

	//matrices
	XMMATRIX worldM;
	XMMATRIX viewM;
	XMMATRIX projM;

	//contains data for a vertex
	struct Vertex
	{
		XMFLOAT4 pos;
		XMFLOAT4 color;
		XMFLOAT2 uv;
		XMFLOAT4 normal;
	};

	//contains data for world, projection, and view matricies
	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	Vertex* obj1 = nullptr;
	int numVertices = 0;
	UINT* indices = nullptr;
	int numIndices = 0;
	float modelScale = 0.5f;

	//timer variables
	XTime timer;
	float deltaT;
	float rotationDegree;
	
	//Test Functions
	//fills array with appropriate vertex info to draw a test triangle
	void Triangle(Vertex** _obj);
	//fills array with appropriate vertex info to draw a test cube
	void Cube(Vertex** _obj);
	
	//process vertex information from OBJ file
	void LoadOBJVerts(const char* _filename, Vertex** _obj, UINT** _indList);


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

			//LOAD OBJECT ONTO THE VIDEO CARD////////////////////////////////////
			//Cube(&obj1);
			LoadOBJVerts("dead_tree1.txt", &obj1, &indices);

			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData;
			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));

			//VERTEX BUFFER
			//set up buffer description
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.ByteWidth = sizeof(Vertex) * numVertices;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			bDesc.Usage = D3D11_USAGE_DEFAULT; //would change if constantly rewriting a constant buffer


			//set up subdata
			subData.pSysMem = obj1;

			//create vertex buffer
			hr = myDevice->CreateBuffer(&bDesc, &subData, &vBuffer);

			//INDEX BUFFER
			bDesc.ByteWidth = sizeof(UINT) * numIndices;
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			subData.pSysMem = indices;
			hr = myDevice->CreateBuffer(&bDesc, &subData, &iBuffer);

			//CONSTANT BUFFER
			bDesc.ByteWidth = sizeof(ConstantBuffer);
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			hr = myDevice->CreateBuffer(&bDesc, nullptr, &cBuffer);

			// Initialize the world matrix
			worldM = XMMatrixIdentity();

			//Initialize the view matrix
			XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
			XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewM = XMMatrixLookAtLH(Eye, At, Up);

			//Initialize the projection matrix
			projM = XMMatrixPerspectiveFovLH(XM_PIDIV2, vpWidth / (FLOAT)vpHeight, 0.01f, 100.0f);

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
				{"NORMAL", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			//input layout (day 1 vid)
			hr = myDevice->CreateInputLayout(ieDesc, 4, MyVShader, sizeof(MyVShader), &vLayout);
			////////////////////////////////////////////////////////////////////

			//reset timer variables
			timer.Restart();
			deltaT = 0;
			rotationDegree = 0;

			
		}
	}
}

//fills array with appropriate vertex info to draw a test triangle
void LetsDrawSomeStuff::Triangle(Vertex** _obj)
{
	Vertex* temp;
	temp = new Vertex[3];
	temp[0].pos.x = 0;
	temp[0].pos.y = 0.5f;
	temp[0].pos.z = 0;
	temp[0].pos.w = 1;
	temp[0].color.x = 1;
	temp[0].color.y = 1;
	temp[0].color.z = 1;
	temp[0].color.w = 1;
	temp[0].uv.x = 0;
	temp[0].uv.y = 0;

	temp[1].pos.x = 0.5f;
	temp[1].pos.y = -0.5f;
	temp[1].pos.z = 0;
	temp[1].pos.w = 1;
	temp[1].color.x = 1;
	temp[1].color.y = 1;
	temp[1].color.z = 1;
	temp[1].color.w = 1;
	temp[1].uv.x = 0;
	temp[1].uv.y = 0;

	temp[2].pos.x = -0.5f;
	temp[2].pos.y = -0.5f;
	temp[2].pos.z = 0;
	temp[2].pos.w = 1;
	temp[2].color.x = 1;
	temp[2].color.y = 1;
	temp[2].color.z = 1;
	temp[2].color.w = 1;
	temp[2].uv.x = 0;
	temp[2].uv.y = 0;

	*_obj = temp;

	numVertices = 3;
}

//fills array with appropriate vertex info to draw a test cube
void LetsDrawSomeStuff::Cube(Vertex** _obj)
{
	//XMFLOAT4 CubeColor = { rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), 1.0f };
	XMFLOAT4 CubeColor = RAND_COLOR;
	Vertex* temp;
	//temp = new Vertex[36];

	////front of cube 0-5
	//temp[0] = { XMFLOAT4(-0.25f, 0.25f, -0.25f, 1.0f), CubeColor, XMFLOAT2(0.0f, 0.0f) };
	//temp[1] = { XMFLOAT4(-0.25f,  0.25f, -0.25f, 1.0f), CubeColor, XMFLOAT2(0.0f, 0.0f) };
	//temp[2] = { XMFLOAT4(0.25f,  0.25f, -0.25f, 1.0f), CubeColor, XMFLOAT2(1.0f, 0.0f) };
	//temp[3] = { XMFLOAT4(-0.25f, -0.25f, -0.25f, 1.0f), CubeColor, XMFLOAT2(0.0f, 1.0f) };
	//temp[4] = { XMFLOAT4(0.25f, -0.25f, -0.25f, 1.0f), CubeColor, XMFLOAT2(1.0f, 1.0f) };
	//temp[5] = { XMFLOAT4(0.25f, -0.25f, -0.25f, 1.0f), CubeColor, XMFLOAT2(1.0f, 1.0f) };

	//	//right side of cube 6-11
	//temp[6] = { XMFLOAT4(0.25f, 0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[7] = { XMFLOAT4(0.25f, 0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[8] = { XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 0.0f) };
	//temp[9] = { XMFLOAT4(0.25f, -0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 1.0f) };
	//temp[10] = { XMFLOAT4(0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };
	//temp[11] = { XMFLOAT4(0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };

	//	//back of cube 12-17
	//temp[12] = { XMFLOAT4(0.25f,  0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[13] = { XMFLOAT4(0.25f,  0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[14] = { XMFLOAT4(-0.25f,  0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 0.0f) };
	//temp[15] = { XMFLOAT4(0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 1.0f) };
	//temp[16] = { XMFLOAT4(-0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };
	//temp[17] = { XMFLOAT4(-0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };

	//	//left side of cube 18-23
	//temp[18] = { XMFLOAT4(-0.25f, 0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[19] = { XMFLOAT4(-0.25f, 0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[20] = { XMFLOAT4(-0.25f, 0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 0.0f) };
	//temp[21] = { XMFLOAT4(-0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 1.0f) };
	//temp[22] = { XMFLOAT4(-0.25f, -0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };
	//temp[23] = { XMFLOAT4(-0.25f, -0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };

	//	//top of cube 24-29
	//temp[24] = { XMFLOAT4(-0.25f, 0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[25] = { XMFLOAT4(-0.25f, 0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[26] = { XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 0.0f) };
	//temp[27] = { XMFLOAT4(-0.25f, 0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 1.0f) };
	//temp[28] = { XMFLOAT4(0.25f, 0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };
	//temp[29] = { XMFLOAT4(0.25f, 0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };

	//	//bottom of cube 30-35
	//temp[30] = { XMFLOAT4(-0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[31] = { XMFLOAT4(-0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 0.0f) };
	//temp[32] = { XMFLOAT4(0.25f, -0.25f, 0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 0.0f) };
	//temp[33] = { XMFLOAT4(-0.25f, -0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(0.0f, 1.0f) };
	//temp[34] = { XMFLOAT4(0.25f, -0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };
	//temp[35] = { XMFLOAT4(0.25f, -0.25f, -0.25f, 1.0f), CubeColor,  XMFLOAT2(1.0f, 1.0f) };

	temp = new Vertex[8];
	
	temp[0] = { XMFLOAT4(-0.5f, 0.5f, -0.5f, 1), CubeColor, XMFLOAT2(0,0) };
	temp[1] = { XMFLOAT4(0.5f, 0.5f, -0.5f, 1), CubeColor, XMFLOAT2(0,0) };
	temp[2] = { XMFLOAT4(0.5f, -0.5f, -0.5f, 1), CubeColor, XMFLOAT2(0,0) };
	temp[3] = { XMFLOAT4(-0.5f, -0.5f, -0.5f, 1), CubeColor, XMFLOAT2(0,0) };
	
	temp[4] = { XMFLOAT4(-0.5f, 0.5f, 0.5f, 1), CubeColor, XMFLOAT2(0,0) };
	temp[5] = { XMFLOAT4(0.5f, 0.5f, 0.5f, 1), CubeColor, XMFLOAT2(0,0) };
	temp[6] = { XMFLOAT4(0.5f, -0.5f, 0.5f, 1), CubeColor, XMFLOAT2(0,0) };
	temp[7] = { XMFLOAT4(-0.5f, -0.5f, 0.5f, 1), CubeColor, XMFLOAT2(0,0) };

	for (int i = 0; i < 8; ++i)
	{
		temp[i].color = RAND_COLOR;
	}

	

	indices = new UINT[36] 
	{
		0,1,3,
		3,1,2,
		0,4,5,
		0,5,1,
		1,5,2,
		2,5,6,
		4,0,7,
		7,0,3,
		7,3,2,
		7,2,6,
		5,4,6,
		6,4,7
	};

	numIndices = 36;

	numVertices = 8;
	*_obj = temp;
}

//process vertex information from OBJ file
void LetsDrawSomeStuff::LoadOBJVerts(const char* _filename, Vertex** _obj, UINT** _indList)
{
	// set up output console
	AllocConsole();
	/*freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);*/

	//dynamic arrays to temporarily store vertex information
	DynArray<XMFLOAT4> posIn;
	DynArray<XMFLOAT2> texIn;
	DynArray<XMFLOAT4> normIn;
	DynArray<XMINT3> indexIn;

	//read in the file
	ifstream inFile;
	inFile.open(_filename);

	//buffer to hold raw file chars
	char buffer[256] = { 0 };

	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			inFile >> buffer;

			if (0 == strcmp(buffer, "v"))
			{
				XMFLOAT4 xyzw;
				xyzw.w = 1;
				inFile >> xyzw.x >> xyzw.y >> xyzw.z;
				/*xyzw.x *= modelScale;
				xyzw.y *= modelScale;
				xyzw.z *= modelScale;*/
				posIn.append(xyzw);
			}
			else if (0 == strcmp(buffer, "vt"))
			{
				XMFLOAT2 uv;
				inFile >> uv.x >> uv.y;
				texIn.append(uv);
			}
			else if (0 == strcmp(buffer, "vn"))
			{
				XMFLOAT4 xyzw;
				xyzw.w = 0.0f;
				inFile >> xyzw.x >> xyzw.y >> xyzw.z;
				normIn.append(xyzw);
			}
			else if (0 == strcmp(buffer, "f"))
			{
				//variable to store pos(x)/texture(y)/normal(z) indices
				XMINT3 val;

				for (int i = 0; i < 3; ++i)
				{
					inFile >> val.x;
					val.x -= 1;
					inFile.ignore();

					inFile >> val.y;
					val.y -= 1;
					inFile.ignore();

					inFile >> val.z;
					val.z -= 1;
					indexIn.append(val);
				}
				
			}
		}
		
		//debugging output
		if (_DEBUG)
		{
			// print read in positions
			/*cout << "Positions (x y z)\n";
			for (int i = 0; i < posIn.size(); ++i)
			{
				cout << posIn[i].x << ' ' << posIn[i].y << ' ' << posIn[i].z << '\n';
			}*/

			//print read in textures
			cout << "Textures (u v)\n";
			for (int i = 0; i < posIn.size(); ++i)
			{
				cout << texIn[i].x << ' ' << texIn[i].y << '\n';
			}
		}

		//store the number of indices read in
		numIndices = indexIn.size();
		numVertices = 0;
		//temporary vertex and index array
		Vertex* verts = new Vertex[numIndices];
		UINT* inds = new UINT[numIndices];
		//populate new vertex array with read in data
		for (int i = 0; i < numIndices; ++i)
		{
			verts[i].pos = posIn[indexIn[i].x];
			verts[i].uv = texIn[indexIn[i].y];
			verts[i].normal = normIn[indexIn[i].z];

			inds[i] = indexIn[i].x;

			verts[i].color = { 1,0,0,1 };
			++numVertices;
		}

		//assign temp vert array to array param
		*_obj = verts;
		*_indList = inds;
	}
	inFile.close();
	

	
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
	iBuffer->Release();
	cBuffer->Release();
	vLayout->Release();
	vShader->Release();
	pShader->Release();

	//delete dynamic memory
	delete[] obj1;
	delete[] indices;

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
		timer.Signal();
		deltaT += (float)timer.Delta();

		if (deltaT > (1.0f / 60.0f))
		{
			//update the degree of the object's rotation
			rotationDegree += 0.02f;
			if (rotationDegree >= 360)
				rotationDegree = 0;
			deltaT -= deltaT;
		}

		//rotate object
		worldM = XMMatrixRotationY(rotationDegree);

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
			const float black[] = { 0,0,0,1 };
			myContext->ClearRenderTargetView(myRenderTargetView, black);

			//update constant buffer
			ConstantBuffer conBuff;
			conBuff.world = XMMatrixTranspose(worldM);
			conBuff.view = XMMatrixTranspose(viewM);
			conBuff.projection = XMMatrixTranspose(projM);
			myContext->UpdateSubresource(cBuffer, 0, nullptr, &conBuff, 0, 0);

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
			myContext->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //what do we want it to draw? line, triangle, etc.

			//vertex shader stage
			myContext->VSSetShader(vShader, 0, 0);
			myContext->VSSetConstantBuffers(0, 1, &cBuffer);

			//pixel shader stage
			myContext->PSSetShader(pShader, 0, 0);
			

			//Draw (nothing actually happens until draw is called)
			myContext->DrawIndexed(numIndices, 0, 0);

			/////////////////////////////////////////////////////////////////////////////

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}