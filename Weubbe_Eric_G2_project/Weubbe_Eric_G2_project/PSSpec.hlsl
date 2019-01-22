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
	matrix TreeInstPositions[3];
}

texture2D tree : register(t0);

SamplerState treeFilter : register(s0); //default provided does clamping an bilinear filter

float4 main(PSVertex _input) : SV_TARGET
{
	float4 color = float4(0,0,0,0);

	//apply directional light (light[0])
	color += saturate(dot(lightDir[0], _input.normal) * lightCol[0]);
	//apply ambient light
	color = lerp(float4(0, 0, 0, 1), color, color + 0.9f);

	//apply point light (light[1])
	float4 pointDir = normalize(lightDir[1] - _input.worldPos);
	float lightRatio = saturate(dot(pointDir, _input.normal));
	float atten = 1.0f - saturate((length(lightDir[1] - _input.worldPos)) / pointRad);
	color += lightCol[1] * lightRatio * atten;

	//apply spot light (light[2])
	float3 spotDir = normalize(lightDir[2] - _input.worldPos);
	float surfaceRatio = saturate(dot(-spotDir, coneDir));
	atten = 1.0f - saturate((coneRatio - surfaceRatio) / (coneRatio - 0.8f));
	lightRatio = saturate(dot(spotDir, _input.normal));
	color += lightRatio * lightCol[2] * atten;

	//apply specular
	float3 viewDir = normalize(_input.boxPos - _input.worldPos);
	float3 halfVec = normalize((spotDir) - viewDir);
	float intensity = max(pow(saturate(dot(_input.normal, normalize(halfVec))), 2), 0);
	color += lightCol[2] * intensity * 50;

	halfVec = normalize((lightDir[0])-viewDir);
	intensity = max(pow(saturate(dot(_input.normal, normalize(halfVec))), 2), 0);
	color += lightCol[0] * intensity * 50;

	//texture object
	color *= tree.Sample(treeFilter, _input.uv);
	 if (color.w == 0)
	 {
		 discard;
	 }

	 return color;
}