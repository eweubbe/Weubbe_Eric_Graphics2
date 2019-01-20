struct PSVertex
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD1;
	float4 normal : NORMAL;
	float3 boxPos : TEXCOORD3;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 lightDir[3];
	float4 lightCol[3];
	float4 outputCol;
	float pointRad;
	float coneRatio;
	float4 coneDir;
	matrix TreeInstPositions[3];
}

float4 main(PSVertex _input) : SV_TARGET
{
	 return _input.color;
}