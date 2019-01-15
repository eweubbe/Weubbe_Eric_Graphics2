//super basic pixel shader
struct PSVertex
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD1;
	float4 normal : NORMAL;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 lightDir[2];
	float4 lightCol[2];
	float4 outputCol;
}

texture2D tree : register(t0);

SamplerState treeFilter : register(s0); //default provided does clamping an bilinear filter

float4 main(PSVertex _input) : SV_TARGET
{
	//float4 color = 0;

	//color += saturate(dot(lightDir[0], _input.normal) * lightCol[0]);

	//color *= tree.Sample(treeFilter, _input.uv);

	//return color *= _input.color;

	return tree.Sample(treeFilter, _input.uv);
	//return _input.color; 
}