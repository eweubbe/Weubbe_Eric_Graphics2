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
	float4 pos[1] :SV_POSITION;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(4)]
void main(point float4 _input[1]: SV_POSITION, inout TriangleStream < GSOutput > output)
{
	GSOutput verts[4] =
	{
		float4(0,0,0,1),
		float4(0,0,0,1),
		float4(0,0,0,1),
		float4(0,0,0,1)
	};

	verts[0].pos = _input[0];

	verts[1].pos = _input[0];
	verts[1].pos.y = _input[0].y - 0.5f;

	verts[2].pos = _input[0];
	verts[2].pos.x = _input[0].x - 0.5f;

	verts[3].pos = _input[0];
	verts[3].pos.x = _input[0].x - 0.5f;
	verts[3].pos.y = _input[0].y - 0.5f;

	matrix mVP = mul(view, projection);

	for (uint i = 0; i < 4; ++i)
		verts[i].pos = mul(verts[i].pos, mVP);
	
	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
	output.Append(verts[3]);
}