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
}

textureCUBE sky : register(t0);

SamplerState skyFilter : register(s0);

float4 main(PSVertex _input) : SV_TARGET
{
	float4 color = float4(0,0,0,0);
	float3 viewDirection = _input.boxPos - (_input.worldPos).xyz;
	float3 reflection = reflect(-normalize(viewDirection), normalize(_input.normal));
	color += sky.Sample(skyFilter,normalize(reflection));
	return color;
}