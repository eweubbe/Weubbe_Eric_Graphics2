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

struct InputVertex
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
	float4 normal : NORMAL;
};

struct PSVertex
{
	float4 pos : SV_POSITION; //SV: system value
	float4 color : COLOR;
	float2 uv : TEXCOORD1;
	float4 normal : NORMAL;
	float4 worldPos : TEXCOORD2;
	float3 boxPos : TEXCOORD3;
};

PSVertex main(InputVertex _input, uint instanceID : SV_InstanceID)
{
	PSVertex output = (PSVertex)0;
	//position
	output.pos = mul(TreeInstPositions[instanceID],_input.pos);
	output.worldPos = output.pos;
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	//lighting
	_input.normal.w = 0.0f;
	output.normal = mul(TreeInstPositions[instanceID], _input.normal);


	//texture
	output.uv = _input.uv;

	output.color = _input.color;

	return output;
}