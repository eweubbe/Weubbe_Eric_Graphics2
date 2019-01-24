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

struct GSVertex
{
	float4 pos: POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD1;
	float4 normal : NORMAL;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD2;
	float4 normal : NORMAL;
};

[maxvertexcount(4)]
void main(point GSVertex _input[1], inout TriangleStream < GSOutput > output)
{
	GSOutput verts[4];

	//positions
	verts[0].pos = _input[0].pos;

	verts[1].pos = _input[0].pos;
	verts[1].pos.y = _input[0].pos.x - 0.5f;

	verts[2].pos = _input[0].pos;
	verts[2].pos.x = _input[0].pos.y - 0.5f;

	verts[3].pos = _input[0].pos;
	verts[3].pos.x = _input[0].pos.x - 0.5f;
	verts[3].pos.y = _input[0].pos.y - 0.5f;

	//UVs
	verts[0].uv = float2(1, 0);
	verts[1].uv = float2(0, 0);
	verts[2].uv = float2(1, 1);
	verts[3].uv = float2(0, 1);

	for (int i = 0; i < 4; ++i)
	{
		verts[i].pos = mul(verts[i].pos, world);

		//normals
		verts[i].normal = mul(verts[i].normal, world);
	}

	matrix mVP = mul(view, projection);

	for (uint i = 0; i < 4; ++i)
		verts[i].pos = mul(verts[i].pos, mVP);
	
	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
	output.Append(verts[3]);
}