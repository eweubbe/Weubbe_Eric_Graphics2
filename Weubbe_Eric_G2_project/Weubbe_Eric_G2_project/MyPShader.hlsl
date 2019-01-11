//super basic pixel shader
struct InputVertex
{
	float4 pos : SV_POSITION; //SV: system value
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

float4 main(InputVertex _input) : SV_TARGET
{
	 return _input.color; // outputs red
}