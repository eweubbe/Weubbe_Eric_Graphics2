//super basic pixel shader
struct InputVertex
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
	float4 normal : NORMAL;
};

texture2D tree : register(t0);

SamplerState treeFilter : register(s0); //default provided does clamping an bilinear filter

float4 main(float4 _pos : SV_POSITION, float4 _col : COLOR, float2 _uv : TEXCOORD0) : SV_TARGET
{
	return tree.Sample(treeFilter, _uv);
	//return _input.color; 
}