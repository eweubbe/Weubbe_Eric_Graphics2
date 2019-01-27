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
	color = lerp(float4(0, 0, 0, 1), color, color + 0.9f);

	//apply point light (light[1])
	/*float4 pointDir = normalize(lightDir[1] - _input.worldPos);
	float lightRatio = saturate(dot(pointDir, _input.normal));
	float atten = 1.0f - saturate((length(lightDir[1] - _input.worldPos)) / pointRad);
	color += lightCol[1] * lightRatio * atten;*/

	//apply point lights
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
	color += lightRatio * lightCol[2] * atten;

	//apply specular
	/*float3 viewDir = normalize(camPos - _input.worldPos);
	float3 halfVec = normalize((spotDir) + viewDir);
	float intensity = max(pow(saturate(dot(_input.normal, normalize(halfVec))), PowInt.x), 0);
	color += lightCol[2] * intensity * PowInt.y;

	halfVec = normalize((lightDir[0])+viewDir);
	intensity = max(pow(saturate(dot(_input.normal, normalize(halfVec))), PowInt.x), 0);
	color += lightCol[0] * intensity * PowInt.y;*/

	float3 viewDir = normalize((float3)camPos - (float3)_input.worldPos);
	float3 lightDirection;
	float3 reflection;
	float fSpec;
	//apply specular
	if (abs(distance(_input.worldPos, lightDir[2])) <= 15)
	{
		
		lightDirection = normalize((float3)_input.worldPos - (float3)lightDir[2]);
		reflection = reflect(lightDirection, (float3)_input.normal);
		fSpec = saturate(dot(reflection, viewDir));
		fSpec = pow(fSpec, PowInt.x);
		color += lightCol[2] * fSpec * PowInt.y;
	}
	

	lightDirection = (float3)-lightDir[0];
	reflection = reflect(lightDirection, (float3)_input.normal);
	fSpec = saturate(dot(reflection, viewDir));
	fSpec = pow(fSpec, PowInt.x);
	color += lightCol[0] * fSpec * PowInt.y;

	for (int j = 0; j < 8; ++j)
	{
		if (abs(distance(_input.worldPos, fairyPos[j])) <= 15)
		{
			lightDirection = normalize((float3)_input.worldPos - (float3)fairyPos[j]);
			reflection = reflect(lightDirection, (float3)_input.normal);
			fSpec = saturate(dot(reflection, viewDir));
			fSpec = pow(fSpec, PowInt.x * 100);
			color += lightCol[1] * fSpec * PowInt.y *0.5f;
		}
	}

	//texture object
	color *= tree.Sample(treeFilter, _input.uv);
	 if (color.w == 0)
	 {
		 discard;
	 }

	 return color;
}