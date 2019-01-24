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

//struct GSVertex
//{
//	float4 pos[1] :SV_POSITION;
//};

void main(InputVertex _input, out float4 result[1] : SV_POSITION)
{
	//GSVertex output = (GSVertex)0;
	//
	////position
	//output.pos[0] = mul(_input.pos, world);

	//return output;

	float4 pos[] = { mul(_input.pos, world) };

	result = pos;
}