// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include <d3d11.h>
//#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
#include <directxcolors.h>
#include"XTime.h"
#include<iostream>
#include<fstream>
#include"DynArray.h"
#include"DDSTextureLoader.h"
#include"Gateware Redistribution R5d/Interface/G_System/GKeyDefines.h"
#include"Gateware Redistribution R5d/Interface/G_System/GInput.h"

//include compiled shaders
#include "myVShader.csh"
#include "VS_Instanced.csh"
#include "myPShader.csh"
#include "PSSOLID.csh"
#include "PS_SkyBox.csh"


using namespace DirectX;
using namespace std;
using namespace GW;
using namespace CORE;
using namespace SYSTEM;

//Defines
#define RAND_COLOR XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), 1.0f);
#define EPSILON 0.00001f
#define NUM_OBJECTS 3
#define NUM_LIGHTS 2
#define TREE_INSTANCES 3

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	GW::SYSTEM::GInput* inputs = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	HRESULT hr;
	//video card version of new
	ID3D11Buffer* vBuffer[NUM_OBJECTS];
	ID3D11Buffer* iBuffer[NUM_OBJECTS];
	ID3D11Buffer* cBuffer = nullptr;
	//descirbes what a vertex looks like to directx
	ID3D11InputLayout* vLayout = nullptr;
	//viewport
	D3D11_VIEWPORT myPort;
	//shader variables
	ID3D11VertexShader* vShader = nullptr; //HLSL (high level shading laguage)
	ID3D11VertexShader* InstanceVshader = nullptr;
	ID3D11PixelShader* pShader = nullptr; //HLSL
	ID3D11PixelShader* pSolid = nullptr;
	ID3D11PixelShader* pSkyBox = nullptr;

	//TEXTURE
	ID3D11SamplerState* SamplerLinear = nullptr;
	//tree
	ID3D11Texture2D* treeTex = nullptr; //what we load pixel data into
	ID3D11ShaderResourceView* treeView = nullptr;
	//grass
	ID3D11Texture2D* grassTex = nullptr;
	ID3D11ShaderResourceView* grassView = nullptr;
	//skybox
	ID3D11Texture2D* skyTex = nullptr;
	ID3D11ShaderResourceView* skyView = nullptr;

	//matrices
	XMMATRIX worldM;
	XMMATRIX viewM;
	XMMATRIX projM;
	XMVECTOR viewDet;

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
		XMFLOAT4 LightDir[NUM_LIGHTS];
		XMFLOAT4 LightColor[NUM_LIGHTS];
		XMFLOAT4 OutputColor;
		float pointRad; //point light radius
		XMMATRIX TreeInstPositions[TREE_INSTANCES]; // array of position matrices for instanced trees
	};

	Vertex* objs[NUM_OBJECTS];
	int vertNums[NUM_OBJECTS];
	UINT* indices[NUM_OBJECTS];
	int indNums[NUM_OBJECTS];
	float modelScale = 0.5f;

	//timer variables
	XTime timer;
	float deltaT;
	float rotationDegree;
	//point light varis
	XMFLOAT4 pointPos;
	float pointLightInc;
	//tree position matrices
	XMMATRIX treePos[TREE_INSTANCES];

	//cursor detection
	POINT startingCursorPos;
	
	//Test Functions
	//fills array with appropriate vertex info to draw a test triangle
	//void Triangle(Vertex** _obj);
	//fills array with appropriate vertex info to draw a test cube
	void Cube(UINT _arrPos);
	//generates a plane and passes it to appropriate ind and view lists
	void Plane(UINT _arrPos);
	//process vertex information from OBJ file
	void LoadOBJVerts(const char* _filename, UINT _arrPos);
	void Compactify(UINT _arrPos);


public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
	// Handle Window Resize
	void Resize(GW::SYSTEM::GWindowInputEvents _event, GW::SYSTEM::GWindow* _window);
};

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	if (attatchPoint) // valid window?
	{
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			if (G_SUCCESS(GW::SYSTEM::CreateGInput((void*)(attatchPoint->GetWindowHandle(sizeof(HWND),nullptr)), sizeof(HWND), &inputs)))
			{
				float x = 1;
			}

			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)

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
			
			//MESHES**********************************************************************************
			LoadOBJVerts("dead_tree1.txt", 0);
			Cube(1);
			Plane(2);

			//TEXTURES********************************************************************************
			//dds loader way
			//tree
			hr = CreateDDSTextureFromFile(myDevice, L"t_DeadTree.dds", (ID3D11Resource**)&treeTex, &treeView);
			//grass
			hr = CreateDDSTextureFromFile(myDevice, L"grass_seamless.dds", (ID3D11Resource**)&grassTex, &grassView);
			//skybox
			hr = CreateDDSTextureFromFile(myDevice, L"NightSky.dds", (ID3D11Resource**)&skyTex, &skyView);

			// Create the sample state
			D3D11_SAMPLER_DESC sampDesc = {};
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			hr = myDevice->CreateSamplerState(&sampDesc, &SamplerLinear);
			
			//BUFFERS********************************************************************************
			D3D11_BUFFER_DESC bDesc;
			D3D11_SUBRESOURCE_DATA subData;
			ZeroMemory(&bDesc, sizeof(bDesc));
			ZeroMemory(&subData, sizeof(subData));

			//VERTEX BUFFER
			//set up buffer description
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;
			bDesc.Usage = D3D11_USAGE_DEFAULT; //would change if constantly rewriting a constant buffer
			for (int i = 0; i < NUM_OBJECTS; ++i)
			{
				bDesc.ByteWidth = sizeof(Vertex) * vertNums[i];
				subData.pSysMem = objs[i];
				hr = myDevice->CreateBuffer(&bDesc, &subData, &vBuffer[i]);
			}

			//INDEX BUFFER
			bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			for (int i = 0; i < NUM_OBJECTS; ++i)
			{
				bDesc.ByteWidth = sizeof(UINT) * indNums[i];
				subData.pSysMem = indices[i];
				hr = myDevice->CreateBuffer(&bDesc, &subData, &iBuffer[i]);
			}

			//CONSTANT BUFFER
			bDesc.ByteWidth = sizeof(ConstantBuffer);
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			hr = myDevice->CreateBuffer(&bDesc, nullptr, &cBuffer);

			//MATRICES******************************************************************************
			// Initialize the world matrix
			worldM = XMMatrixIdentity();

			//Initialize the view matrix
			XMVECTOR Eye = XMVectorSet(0.0f, 4.0f, -10.0f, 0.0f);
			XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewM = XMMatrixLookAtLH(Eye, At, Up);

			//convert view matrix back to raw data
			viewDet = XMMatrixDeterminant(viewM);
			viewM = XMMatrixInverse(&viewDet, viewM);

			//Initialize the projection matrix
			projM = XMMatrixPerspectiveFovLH(XMConvertToRadians(65), vpWidth / (FLOAT)vpHeight, 0.01f, 100.0f);

			//SHADERS********************************************************************************
			hr = myDevice->CreateVertexShader(MyVShader, sizeof(MyVShader), nullptr, &vShader);
			hr = myDevice->CreateVertexShader(VS_Instanced, sizeof(VS_Instanced), nullptr, &InstanceVshader);
			hr = myDevice->CreatePixelShader(MyPShader, sizeof(MyPShader), nullptr, &pShader);
			hr = myDevice->CreatePixelShader(PSSOLID, sizeof(PSSOLID), nullptr, &pSolid);
			hr = myDevice->CreatePixelShader(PS_SkyBox, sizeof(PS_SkyBox), nullptr, &pSkyBox);

			//INPUT LAYOUT***************************************************************************
			//input element descriptor, glues c++ vertex struct to hlsl vertex struct
			D3D11_INPUT_ELEMENT_DESC ieDesc[] = 
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{"NORMAL", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			hr = myDevice->CreateInputLayout(ieDesc, 4, MyVShader, sizeof(MyVShader), &vLayout);

			//MISC VARIABLES*************************************************************************
			//reset timer variables
			timer.Restart();
			deltaT = 0;
			rotationDegree = 0;
			pointPos = XMFLOAT4(-5.0f, 3.0f, 6.0f, 1.0f);
			pointLightInc = 0.1f;

			//set instanced tree positions
			treePos[0] = XMMatrixMultiply(XMMatrixTranslation(5.0f, 0.0f, 0.0f), XMMatrixIdentity());
			treePos[1] = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, 0.0f), XMMatrixIdentity());
			treePos[2] = XMMatrixMultiply(XMMatrixTranslation(-5.0f, 0.0f, 0.0f), XMMatrixIdentity());

			//set initial cursor position
			GetCursorPos(&startingCursorPos);

			
		}
	}
}

//fills array with appropriate vertex info to draw a test triangle
//void LetsDrawSomeStuff::Triangle(Vertex** _obj)
//{
//	Vertex* temp;
//	temp = new Vertex[3];
//	temp[0].pos.x = 0;
//	temp[0].pos.y = 0.5f;
//	temp[0].pos.z = 0;
//	temp[0].pos.w = 1;
//	temp[0].color.x = 1;
//	temp[0].color.y = 1;
//	temp[0].color.z = 1;
//	temp[0].color.w = 1;
//	temp[0].uv.x = 0;
//	temp[0].uv.y = 0;
//
//	temp[1].pos.x = 0.5f;
//	temp[1].pos.y = -0.5f;
//	temp[1].pos.z = 0;
//	temp[1].pos.w = 1;
//	temp[1].color.x = 1;
//	temp[1].color.y = 1;
//	temp[1].color.z = 1;
//	temp[1].color.w = 1;
//	temp[1].uv.x = 0;
//	temp[1].uv.y = 0;
//
//	temp[2].pos.x = -0.5f;
//	temp[2].pos.y = -0.5f;
//	temp[2].pos.z = 0;
//	temp[2].pos.w = 1;
//	temp[2].color.x = 1;
//	temp[2].color.y = 1;
//	temp[2].color.z = 1;
//	temp[2].color.w = 1;
//	temp[2].uv.x = 0;
//	temp[2].uv.y = 0;
//
//	*_obj = temp;
//
//	//numVertices = 3;
//}

//fills array with appropriate vertex info to draw a test cube
void LetsDrawSomeStuff::Cube(UINT _arrPos)
{
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
	
	float cubeDimension = 30.0f;

	//temp[0] = { XMFLOAT4(-0.1f, 0.1f, 0.1f, 1), CubeColor, XMFLOAT2(0,0) };
	//temp[1] = { XMFLOAT4(0.1f, 0.1f, 0.1f, 1), CubeColor, XMFLOAT2(0,0) };
	//temp[2] = { XMFLOAT4(0.1f, -0.1f, 0.1f, 1), CubeColor, XMFLOAT2(0,0) };
	//temp[3] = { XMFLOAT4(-0.1f, -0.1f, 0.1f, 1), CubeColor, XMFLOAT2(0,0) };
	//
	//temp[4] = { XMFLOAT4(-0.1f, 0.1f, -0.1f, 1), CubeColor, XMFLOAT2(0,0) };
	//temp[5] = { XMFLOAT4(0.1f, 0.1f, -0.1f, 1), CubeColor, XMFLOAT2(0,0) };
	//temp[6] = { XMFLOAT4(0.1f, -0.1f, -0.1f, 1), CubeColor, XMFLOAT2(0,0) };
	//temp[7] = { XMFLOAT4(-0.1f, -0.1f, -0.1f, 1), CubeColor, XMFLOAT2(0,0) };

	temp[0] = { XMFLOAT4(-cubeDimension, cubeDimension, cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };
	temp[1] = { XMFLOAT4(cubeDimension, cubeDimension, cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };
	temp[2] = { XMFLOAT4(cubeDimension, -cubeDimension, cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };
	temp[3] = { XMFLOAT4(-cubeDimension, -cubeDimension, cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };

	temp[4] = { XMFLOAT4(-cubeDimension, cubeDimension, -cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };
	temp[5] = { XMFLOAT4(cubeDimension, cubeDimension, -cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };
	temp[6] = { XMFLOAT4(cubeDimension, -cubeDimension, -cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };
	temp[7] = { XMFLOAT4(-cubeDimension, -cubeDimension, -cubeDimension, 1), CubeColor, XMFLOAT2(0,0) };

	for (int i = 0; i < 8; ++i)
	{
		temp[i].color = RAND_COLOR;
	}

	

	indices[_arrPos] = new UINT[36] 
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

	indNums[_arrPos] = 36;

	vertNums[_arrPos] = 8;
	objs[_arrPos] = temp;
}

//generates a plane and passes it to appropriate ind and view lists
void LetsDrawSomeStuff::Plane(UINT _arrPos)
{
	XMFLOAT4 color = {.5f,.5f,.5f,1.0f};
	Vertex* temp = new Vertex[4];
	

	temp[0] = { XMFLOAT4(-30.0f, 0.0f, -30.0f, 1.0f), color, XMFLOAT2(0,0), XMFLOAT4(0,1,0,1)};
	temp[1] = { XMFLOAT4(-30.0f, 0.0f, 30.0f, 1.0f), color, XMFLOAT2(0,1), XMFLOAT4(0,1,0,1)};
	temp[2] = { XMFLOAT4(30.0f, 0.0f, -30.0f, 1.0f), color, XMFLOAT2(1,0), XMFLOAT4(0,1,0,1)};
	temp[3] = { XMFLOAT4(30.0f, 0.0f, 30.0f, 1.0f), color, XMFLOAT2(1,1), XMFLOAT4(0,1,0,1) };

	vertNums[_arrPos] = 4;
	objs[_arrPos] = temp;

	indices[_arrPos] = new UINT[6]
	{
		0,1,2,
		2,1,3
	};
	indNums[_arrPos] = 6;
}

//process vertex information from OBJ file
void LetsDrawSomeStuff::LoadOBJVerts(const char* _filename, UINT _arrPos)
{
	// set up output console for debugging
	AllocConsole();

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
				xyzw.x *= modelScale;
				xyzw.y *= modelScale;
				xyzw.z *= modelScale;
				posIn.append(xyzw);
			}
			else if (0 == strcmp(buffer, "vt"))
			{
				XMFLOAT2 uv;
				inFile >> uv.x >> uv.y;
				uv.y = abs(1.0f - uv.y);
				texIn.append(uv);
			}
			else if (0 == strcmp(buffer, "vn"))
			{
				XMFLOAT4 xyzw;
				xyzw.w = 1.0f;
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

		//store the number of indices read in
		indNums[_arrPos] = indexIn.size();
		vertNums[_arrPos] = 0;
		//temporary vertex and index array
		Vertex* verts = new Vertex[indNums[_arrPos]];
		UINT* inds = new UINT[indNums[_arrPos]];
		//populate new vertex array with read in data
		for (int i = 0; i < indNums[_arrPos]; ++i)
		{
			verts[i].pos = posIn[indexIn[i].x];
			verts[i].uv = texIn[indexIn[i].y];
			verts[i].normal = normIn[indexIn[i].z];

			inds[i] = i;

			//temporary
			verts[i].color = RAND_COLOR;
			++vertNums[_arrPos];
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
			/*cout << "Textures (u v)\n";
			for (int i = 0; i < texIn.size(); ++i)
			{
				cout << texIn[i].x << ' ' << texIn[i].y << '\n';
			}*/

			//print read in normals
			/*cout << "Normals (x y z)\n";
			for (int i = 0; i < normIn.size(); ++i)
			{
				cout << normIn[i].x << ' ' << normIn[i].y << ' ' << normIn[i].z << '\n';
			}*/

			//cout << "Index (pos tex norm)\n";
			//for (int i = 0; i < indexIn.size(); ++i)
			//{
			//	cout << indexIn[i].x + 1 << ' ' << indexIn[i].y + 1 << ' '<< indexIn[i].z + 1 << '\n';
			//}

			float epsilon = 0.00001f;

			//FINAL VERT ARRAY
			/*cout << "Vertex Data\n";
			for (int i = 0; i < indexIn.size(); ++i)
			{
				if ((abs(verts[i].normal.x - (-0.757055000f)) < epsilon) && (abs(verts[i].normal.y - (0.070595000)) < epsilon) && (abs(verts[i].normal.z - (-0.649526000)) < epsilon))
				{
					cout << "Vertex " << i << ': ' << "Pos: " << verts[i].pos.x << ' ' << verts[i].pos.y << ' ' << verts[i].pos.z <<
						" UV: " << verts[i].uv.x << ' ' << verts[i].uv.y <<
						" Norm: " << verts[i].normal.x << ' ' << verts[i].normal.y << ' ' << verts[i].normal.z << '\n';

					cout << indexIn[i].x + 1 << ' ' << indexIn[i].y + 1 << ' ' << indexIn[i].z + 1 << '\n';

				}
			}
			cout << posIn[178].x << ' ' << posIn[178].y << ' ' << posIn[178].z << '\n';
			cout << texIn[201].x << ' ' << texIn[201].y << '\n';
			cout << normIn[362].x << ' ' << normIn[362].y << ' ' << normIn[362].z << '\n';*/
			
		}

		//assign temp vert array to array param
		objs[_arrPos] = verts;
		indices[_arrPos] = inds;

		//Compactify(_obj, _indList);
	}
	inFile.close();
}

//void LetsDrawSomeStuff::Compactify(Vertex** _obj, UINT** _indList)
//{
//	DynArray<Vertex> vertList;
//
//	Vertex* verts = *_obj;
//	UINT* inds = *_indList;
//
//	float epsilon = 0.0001f;
//
//	for (int i = 0; i < numIndices; ++i)
//	{
//		bool unique = true;
//		int location = -1;
//
//		for (int j = 0; j < vertList.size(); ++j)
//		{
//			if (
//				(abs(verts[inds[i]].pos.x - vertList[j].pos.x) < epsilon) &&
//				(abs(verts[inds[i]].pos.y - vertList[j].pos.y) < epsilon) &&
//				(abs(verts[inds[i]].pos.z - vertList[j].pos.z) < epsilon) &&
//				(abs(verts[inds[i]].normal.x - vertList[j].normal.x) < epsilon) &&
//				(abs(verts[inds[i]].normal.y - vertList[j].normal.y) < epsilon) &&
//				(abs(verts[inds[i]].normal.z - vertList[j].normal.z) < epsilon) &&
//				(abs(verts[inds[i]].uv.x - vertList[j].uv.x) < epsilon) &&
//				(abs(verts[inds[i]].uv.y - vertList[j].uv.y) < epsilon)
//				)
//			{
//				unique = false;
//				location = j;
//				break;
//			}
//		}
//		if (unique)
//		{
//			vertList.append(verts[i]);
//			inds[i] = vertList.size() - 1;
//		}
//		else
//		{
//			inds[i] = location;
//		}
//	}
//
//	//delete[] * _obj;
//	delete[] verts;
//	verts = nullptr;
//	verts = new Vertex[vertList.size()];
//
//	for (int i = 1; i < vertList.size(); ++i)
//	{
//		verts[i] = vertList[i];
//	}
//
//	*_obj = verts;
//	*_indList = inds;
//	numVertices = vertList.size();
//}


// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	// Release DX Objects aquired from the surface
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();

	// TODO: "Release()" more stuff here!
	for (int i = 0; i < NUM_OBJECTS; ++i)
		vBuffer[i]->Release();
	for (int i = 0; i < NUM_OBJECTS; ++i)
		iBuffer[i]->Release();
	cBuffer->Release();
	vLayout->Release();
	vShader->Release();
	InstanceVshader->Release();
	pShader->Release();
	pSolid->Release();
	pSkyBox->Release();
	treeTex->Release();
	treeView->Release();
	grassTex->Release();
	grassView->Release();
	skyTex->Release();
	skyView->Release();
	SamplerLinear->Release();

	//delete dynamic memory
	for (int i = 0; i < NUM_OBJECTS; ++i)
		delete objs[i];
	for (int i = 0; i < NUM_OBJECTS; ++i)
		delete indices[i];

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

// Handle Window Resize
void LetsDrawSomeStuff::Resize(GW::SYSTEM::GWindowInputEvents _event, GW::SYSTEM::GWindow* _window)
{
	//get new client width and height
	UINT width;
	UINT height;
	_window->GetClientWidth(width);
	_window->GetHeight(height);

	if (_event == GW::SYSTEM::GWindowInputEvents::RESIZE)
	{	
		// Grab handles to all DX11 base interfaces
		//myDevice->Release();
		mySwapChain->Release();
		//mySurface->GetDevice((void**)&myDevice);
		mySurface->GetSwapchain((void**)&mySwapChain);

		////REMAKE BUFFERS
		//D3D11_BUFFER_DESC bDesc;
		//D3D11_SUBRESOURCE_DATA subData;
		//ZeroMemory(&bDesc, sizeof(bDesc));
		//ZeroMemory(&subData, sizeof(subData));
		////VERTEX BUFFER
		//	//set up buffer description
		//bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bDesc.CPUAccessFlags = 0;
		//bDesc.MiscFlags = 0;
		//bDesc.StructureByteStride = 0;
		//bDesc.Usage = D3D11_USAGE_DEFAULT; //would change if constantly rewriting a constant buffer
		//for (int i = 0; i < NUM_OBJECTS; ++i)
		//{
		//	bDesc.ByteWidth = sizeof(Vertex) * vertNums[i];
		//	subData.pSysMem = objs[i];
		//	hr = myDevice->CreateBuffer(&bDesc, &subData, &vBuffer[i]);
		//}

		////INDEX BUFFER
		//bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//for (int i = 0; i < NUM_OBJECTS; ++i)
		//{
		//	bDesc.ByteWidth = sizeof(UINT) * indNums[i];
		//	subData.pSysMem = indices[i];
		//	hr = myDevice->CreateBuffer(&bDesc, &subData, &iBuffer[i]);
		//}
	}
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		timer.Signal();
		deltaT += (float)timer.Delta();

		//cursor info
		POINT currCursorPos;
		GetCursorPos(&currCursorPos);
		long deltX = startingCursorPos.x - currCursorPos.x;
		long deltY = startingCursorPos.y - currCursorPos.y;

		XMMATRIX viewCpy = viewM;
		XMMATRIX worldCpy = worldM;

		if (deltaT > (1.0f / 60.0f))
		{
			//update the degree of the object's rotation
			rotationDegree += 0.02f;
			if (rotationDegree >= 360)
				rotationDegree = 0;
			deltaT -= deltaT;

			//update point light pos
			pointPos.z -= pointLightInc;
			if ((pointPos.z >= 6.0f) | (pointPos.z <= -6.0f))
				pointLightInc = -pointLightInc;

			//Get User Input and update Camera
			if (GetAsyncKeyState('W') )
			{
				viewCpy = XMMatrixMultiply(XMMatrixTranslation(0.0f,0.0f,0.1f),viewCpy);
			}
			if (GetAsyncKeyState('S') )
			{
				viewCpy = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, -0.1f), viewCpy);
			}
			if (GetAsyncKeyState('A') )
			{
				viewCpy = XMMatrixMultiply(XMMatrixTranslation(-0.1f, 0.0f, 0.0f), viewCpy);
			}
			if (GetAsyncKeyState('D') )
			{
				viewCpy = XMMatrixMultiply(XMMatrixTranslation(0.1f, 0.0f, 0.0f), viewCpy);
			}
			if (GetAsyncKeyState('T') )
			{
				viewCpy = XMMatrixMultiply(viewCpy, XMMatrixTranslation(0.0f, 0.1f, 0.0f));
			}
			if (GetAsyncKeyState('G'))
			{
				viewCpy = XMMatrixMultiply(viewCpy, XMMatrixTranslation(0.0f, -0.1f, 0.0f));
			}

			if ((abs(deltX) > 1))
			{
				XMVECTOR origPos = viewCpy.r[3];
				XMVECTOR origin = {0.0f, 0.0f, 0.0f, 1.0f};
				viewCpy.r[3] = origin;
				viewCpy = XMMatrixMultiply(viewCpy,XMMatrixRotationY(-(deltX*0.005f)));
				viewCpy =XMMatrixMultiply(viewCpy, XMMatrixTranslation(origPos.m128_f32[0], origPos.m128_f32[1], origPos.m128_f32[2])); 
				startingCursorPos.x = currCursorPos.x;
			}
			if ((abs(deltY) > 1))
			{
				viewCpy = XMMatrixMultiply( XMMatrixRotationX(-(deltY*0.005f)), viewCpy);
				startingCursorPos.y = currCursorPos.y;
			}
		}

		//rotate object
		viewM = XMMatrixInverse(&viewDet, viewCpy);

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

			// Clear the screen to color
			const float d_green[] = { 0, 0.5f, 0, 1 };
			const float black[] = { 0,0,0,1 };
			const float sky[] = { 0.0f, 0.1f, 0.15f, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, sky);

			//set up lighting data
			XMFLOAT4 LightingColors[NUM_LIGHTS] =
			{
				XMFLOAT4(0.2f, 0.4f, 0.6f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)
			};
			XMFLOAT4 LightingDirs[NUM_LIGHTS] =
			{
				XMFLOAT4(0.777f, 0.977f, -0.31f, 1.0f),
				pointPos
			};

			//update constant buffer
			ConstantBuffer conBuff;
			//conBuff.world = XMMatrixTranspose(worldM);
			conBuff.view = XMMatrixTranspose(viewM);
			conBuff.projection = XMMatrixTranspose(projM);
			for (int i = 0; i < NUM_LIGHTS; ++i)
			{
				conBuff.LightColor[i] = LightingColors[i];
				conBuff.LightDir[i] = LightingDirs[i];
			}
			conBuff.OutputColor = XMFLOAT4(0, 0, 0, 0);
			conBuff.pointRad = 5.0f;

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			
			//SET UP THE PIPELINE/////////////////////////////////////////////////////////
			//output merger
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!
			//rasterizer
			myContext->RSSetViewports(1, &myPort);

			//input assembler general info
			myContext->IASetInputLayout(vLayout);
			ID3D11Buffer* tempVB[] = { vBuffer[0] }; // multiple buffers would be for splitting data up, i.e. separate buffers for pos and color
			UINT strides[] = {sizeof(Vertex)}; //distance between 2 vertecies
			UINT offsets[] = {0}; //where to start from in array

			ID3D11ShaderResourceView* srvs[] = { skyView };


			//draw skybox
			viewDet = XMMatrixDeterminant(viewM);
			XMMATRIX temp = XMMatrixInverse(&viewDet, viewM);
			temp = XMMatrixMultiply(temp, worldCpy);
			worldCpy.r[3] = temp.r[3];
			worldM = worldCpy;
			conBuff.world = XMMatrixTranspose(worldM);
			myContext->UpdateSubresource(cBuffer, 0, nullptr, &conBuff, 0, 0);
			tempVB[0] = vBuffer[1];
			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
			myContext->IASetIndexBuffer(iBuffer[1], DXGI_FORMAT_R32_UINT, 0);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			myContext->VSSetConstantBuffers(0, 1, &cBuffer);
			myContext->VSSetShader(vShader, 0, 0);
			//geometry shader
			myContext->PSSetShaderResources(0, 1, srvs);
			myContext->PSSetSamplers(0, 1, &SamplerLinear);
			myContext->PSSetConstantBuffers(0, 1, &cBuffer);
			myContext->PSSetShader(pSkyBox, 0, 0);
			myContext->DrawIndexed(indNums[1], 0, 0);
			//clear z buffer again
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
				myDepthStencilView->Release();
			}

			//draw tree
			worldM = XMMatrixIdentity();
			worldCpy = worldM;
			worldM = worldCpy;
			conBuff.world = XMMatrixTranspose(worldM);
			for (int i = 0; i < TREE_INSTANCES; ++i)
			{
				conBuff.TreeInstPositions[i] = treePos[i];
			}
			myContext->UpdateSubresource(cBuffer, 0, nullptr, &conBuff, 0, 0);
			tempVB[0] = vBuffer[0];
			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
			myContext->IASetIndexBuffer(iBuffer[0], DXGI_FORMAT_R32_UINT, 0);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //what do we want it to draw? line, triangle, etc.
			myContext->VSSetConstantBuffers(0, 1, &cBuffer);
			myContext->VSSetShader(InstanceVshader, 0, 0);
			srvs[0] = { treeView };
			myContext->PSSetShaderResources(0, 1, srvs);
			myContext->PSSetSamplers(0, 1, &SamplerLinear);
			myContext->PSSetConstantBuffers(0, 1, &cBuffer);
			myContext->PSSetShader(pShader, 0, 0);
			myContext->DrawIndexedInstanced(indNums[0], 3, 0, 0, 0);


			//draw plane
			worldM = XMMatrixIdentity();
			worldCpy = worldM;
			worldM = worldCpy;
			conBuff.world = XMMatrixTranspose(worldM);
			myContext->UpdateSubresource(cBuffer, 0, nullptr, &conBuff, 0, 0);
			tempVB[0] = vBuffer[2];
			myContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
			myContext->IASetIndexBuffer(iBuffer[2], DXGI_FORMAT_R32_UINT, 0);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			myContext->VSSetConstantBuffers(0, 1, &cBuffer);
			myContext->VSSetShader(vShader, 0, 0);
			//geometry shader
			srvs[0] = grassView;
			myContext->PSSetShaderResources(0, 1, srvs);
			myContext->PSSetSamplers(0, 1, &SamplerLinear);
			myContext->PSSetConstantBuffers(0, 1, &cBuffer);
			myContext->PSSetShader(pShader, 0, 0);
			myContext->DrawIndexed(indNums[2], 0, 0);

			/////////////////////////////////////////////////////////////////////////////

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			//reset view matrix
			viewDet = XMMatrixDeterminant(viewM);
			viewM = XMMatrixInverse(&viewDet, viewM);
			worldM = XMMatrixIdentity();

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}