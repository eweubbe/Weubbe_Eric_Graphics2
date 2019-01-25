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
}

float4 main(float4 inPos : SV_POSITION) : SV_TARGET
{
	float4 color = (0.5f, 0.5f, 0.5f, 0.5f);
	////apply directional light (light[0])
	//color += saturate(dot(lightDir[0], _input.normal) * lightCol[0]);
	////apply ambient light
	//color = lerp(float4(0, 0, 0, 1), color, color + 0.9f);

	////apply point light (light[1])
	//float4 pointDir = normalize(lightDir[1] - _input.worldPos);
	//float lightRatio = saturate(dot(pointDir, _input.normal));
	//float atten = 1.0f - saturate((length(lightDir[1] - _input.worldPos)) / pointRad);
	//color += lightCol[1] * lightRatio * atten;

	return color;
}