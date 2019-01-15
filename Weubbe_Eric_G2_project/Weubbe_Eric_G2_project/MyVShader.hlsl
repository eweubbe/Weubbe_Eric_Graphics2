//end of day 2 videos for hlsl primer

//rule of three
// three things must match (DATA)
// 1) C++ Vertex struct 
// 2) input layout
// 3) HLSL Vertex struct

//#pragma pack_matrix( row_major )

cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 lightDir[2];
	float4 lightCol[2];
	float4 outputCol;
}

struct InputVertex
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
	float4 normal : NORMAL;
};

struct PSVertex
{
	float4 pos : SV_POSITION; //SV: system value
	float4 color : COLOR;
	float2 uv : TEXCOORD1;
	float4 normal : NORMAL;
};

PSVertex main(InputVertex _input)
{
	PSVertex output = (PSVertex)0;
	//position
	output.pos = mul(_input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	//lighting
	_input.normal.w = 1.0f;
	output.normal = mul(_input.normal, world);
	

	//texture
	output.uv = _input.uv;
	
	output.color = _input.color;

	return output;
}