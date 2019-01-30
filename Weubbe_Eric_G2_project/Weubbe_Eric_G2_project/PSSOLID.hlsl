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
textureCUBE sky : register(t1);
SamplerState treeFilter : register(s0);

float4 main(PSVertex _input) : SV_TARGET
{
	float4 tex = tree.Sample(treeFilter, _input.uv);
	if (tex.w == 0.0f)
	{
		discard;
	}
	float4 color = float4(0, 0, 0, 0);
	float3 viewDirection = _input.boxPos - (_input.worldPos).xyz;
	float3 reflection = reflect(-normalize(viewDirection), normalize(_input.normal));
	color += sky.Sample(treeFilter, normalize(reflection));

	for (int i = 0; i < 8; ++i)
	{
		float4 pointDir = normalize(fairyPos[i] - _input.worldPos);
		float lightRatio = saturate(dot(pointDir, _input.normal));
		float atten = 1.0f - saturate((length(fairyPos[i] - _input.worldPos)) / pointRad);
		color += lightCol[1] * lightRatio * atten;
	}

	float3 viewDir = normalize((float3)camPos - (float3)_input.worldPos);
	float3 lightDirection = normalize((float3)_input.worldPos - (float3)lightDir[2]);
	float3 reflection2 = reflect(lightDirection, reflection);
	float fSpec = saturate(dot(reflection2, viewDir));
	fSpec = pow(fSpec, PowInt.x);
	color += lightCol[2] * fSpec * PowInt.y;

	lightDirection = (float3) - lightDir[0];
	reflection2 = reflect(lightDirection, reflection);
	fSpec = saturate(dot(reflection2, viewDir));
	fSpec = pow(fSpec, PowInt.x);
	color += lightCol[0] * fSpec * PowInt.y;

	color = color * tex * 2;

	return color;
}