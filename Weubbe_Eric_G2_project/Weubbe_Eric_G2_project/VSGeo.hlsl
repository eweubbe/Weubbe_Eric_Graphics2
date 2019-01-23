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
	matrix TreeInstPositions[3];
}

struct InputVertex
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
	float4 normal : NORMAL;
};

struct GSVertex
{
	float4 pos :SV_POSITION;
};

GSVertex main(InputVertex _input)
{
	GSVertex output = (GSVertex)0;
	
	//position
	output.pos = mul(_input.pos, world);

	return output;
}