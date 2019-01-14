//super basic pixel shader
struct PSVertex
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
	float4 normal : NORMAL;
};

texture2D tree : register(t0);

SamplerState treeFilter : register(s0); //default provided does clamping an bilinear filter

float4 main(PSVertex _input) : SV_TARGET
{
	return tree.Sample(treeFilter, _input.uv);
	//return _input.color; 
}