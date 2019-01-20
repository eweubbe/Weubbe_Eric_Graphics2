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
	float pointRad;
	float coneRatio;
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
	color = lerp(float4(0, 0, 0, 1), color, color * 0.8f );
	
	//apply point light (light[1])
	float4 pointDir = normalize(lightDir[1] - _input.worldPos);
	float lightRatio = saturate(dot(pointDir, _input.normal));
	float atten = 1.0f - saturate((length(lightDir[1] - _input.worldPos)) / pointRad);
	color += lightCol[1] * lightRatio * atten;
	
	
	//texture object
	color *= tree.Sample(treeFilter, _input.uv);
	 if (color.w == 0)
	 {
		 discard;
	 }
	
	 return color;
}