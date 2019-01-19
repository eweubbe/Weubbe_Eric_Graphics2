struct PSVertex
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD1;
	float4 normal : NORMAL;
	float4 worldPos : TEXCOORD2;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 lightDir[2];
	float4 lightCol[2];
	float4 outputCol;
	float pointRad;
	matrix TreeInstPositions[3];
}

texture2D tree : register(t0);

SamplerState skyFilter : register(s0); //default provided does clamping an bilinear filter

float4 main(PSVertex _input) : SV_TARGET
{
	float4 color = float4(0,0,0,0);

	color += tree.Sample(skyFilter, _input.worldPos);

	 return color;
}