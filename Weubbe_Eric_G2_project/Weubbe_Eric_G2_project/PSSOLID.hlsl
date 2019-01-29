struct PSVertex
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD1;
	float4 normal : NORMAL;
	float4 worldPos : TEXCOORD2;
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
	float3 pad;
	float pointRad;
	float3 pad2;
	float coneRatio;
	float4 coneDir;
	matrix TreeInstPositions[16];
	float4 camPos;
	float2 pad3;
	float2 PowInt;
	float4 fairyPos[8];
	float deltaT;
}

texture2D tree : register(t0);

SamplerState treeFilter : register(s0);

float4 main(PSVertex _input) : SV_TARGET
{
	float4 tex = tree.Sample(treeFilter, _input.uv);
	if (tex.w == 0.0f)
	{
		discard;
	}
	return tex;
}