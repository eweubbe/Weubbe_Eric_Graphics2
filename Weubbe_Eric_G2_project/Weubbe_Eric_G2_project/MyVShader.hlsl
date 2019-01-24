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
	float4 lightDir[3];
	float4 lightCol[3];
	float4 outputCol;
	float3 pad;
	float pointRad;
	float3 pad2;
	float coneRatio;
	float4 coneDir;
	matrix TreeInstPositions[16];
	float4 camPos;
	float2 pad3;
	float2 PowInt;
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
	float4 worldPos : TEXCOORD2;
	float3 boxPos : TEXCOORD3;
};

PSVertex main(InputVertex _input)
{
	PSVertex output = (PSVertex)0;
	output.boxPos = (_input.pos).xyz;
	//position
	output.pos = mul(_input.pos, world);
	output.worldPos = output.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	//lighting
	_input.normal.w = 0.0f;
	output.normal = mul(_input.normal, world);
	

	//texture
	output.uv = _input.uv;
	
	output.color = _input.color;

	return output;
}