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

struct PSInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD2;
	float3 normal : NORMAL;
	float4 worldPos : TEXCOORD3;
};

float4 main(PSInput _input) : SV_TARGET
{
	float4 color = (0.0f, 0.0f, 0.0f, 0.0f);
	//apply directional light (light[0])
	color += saturate(dot(lightDir[0], _input.normal) * lightCol[0]);
	//apply ambient light
	color = lerp(float4(0, 0, 0, 1), color, color + 0.9f);

	for (int i = 0; i < 8; ++i)
	{
		float4 pointDir = normalize(fairyPos[i] - _input.worldPos);
		float lightRatio = saturate(dot(pointDir, _input.normal));
		float atten = 1.0f - saturate((length(fairyPos[i] - _input.worldPos)) / pointRad);
		color += lightCol[1] * lightRatio * atten;
	}

	color *= float4(0.7f, 0.7f, 0.7f, 0.5f);

	return color;
}