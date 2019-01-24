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
}

float4 main(float4 inPos[1] : SV_POSITION) : SV_TARGET
{
	return float4(0.5f, 0.5f, 0.5f, 0.5f);
}