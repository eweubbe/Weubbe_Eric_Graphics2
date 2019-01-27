//super basic pixel shader
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

SamplerState treeFilter : register(s0); //default provided does clamping an bilinear filter

float4 main(PSVertex _input) : SV_TARGET
{
	float4 color = float4(0,0,0,0);

	//apply directional light (light[0])
	color += saturate(dot(lightDir[0], _input.normal) * lightCol[0]);
	//apply ambient light
	color = lerp(float4(0, 0, 0, 1), color, color + 0.9f );
	
	////apply point light (light[1])
	//float4 pointDir = normalize(lightDir[1] - _input.worldPos);
	//float lightRatio = saturate(dot(pointDir, _input.normal));
	//float atten = 1.0f - saturate((length(lightDir[1] - _input.worldPos)) / pointRad);
	//color += lightCol[1] * lightRatio * atten;

	for (int i = 0; i < 8; ++i)
	{
		float4 pointDir = normalize(fairyPos[i] - _input.worldPos);
		float lightRatio = saturate(dot(pointDir, _input.normal));
		float atten = 1.0f - saturate((length(fairyPos[i] - _input.worldPos)) / pointRad);
		color += lightCol[1] * lightRatio * atten;
	}

	//apply spot light (light[2])
	float3 spotDir = normalize(lightDir[2] - _input.worldPos);
	float surfaceRatio = saturate(dot(-spotDir, coneDir));
	float atten = 1.0f - saturate((coneRatio - surfaceRatio) / (coneRatio - 0.1f));
	float lightRatio = saturate(dot(spotDir, _input.normal));
	color +=  lightRatio * lightCol[2] *atten;
	
	//texture object
	color *= tree.Sample(treeFilter, _input.uv);
	 if (color.w == 0)
	 {
		 discard;
	 }
	
	 float dist = abs(distance(_input.worldPos, float4(0, 0, 0, 1)));
	 //if ( dist >= 20)
	 {
		 color = lerp(float4(0.0118f, 0.0196f, 0.0392f, 1), color, (1 - dist/30.0f));
	 }

	 return color;
}