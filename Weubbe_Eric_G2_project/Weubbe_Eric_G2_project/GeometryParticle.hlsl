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

//structure in buffer
struct Particle
{
	float4 pos;
	float3 norm;
};

//shader doesn't need input, so pass in empty struct
struct GSin
{

};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD2;
	float3 normal : NORMAL;
};

StructuredBuffer<Particle> buffIn : register(t0);

[maxvertexcount(4)]
void main(point GSin _input[1], uint index : SV_PrimitiveID, inout TriangleStream < GSOutput > output)
{
	GSOutput verts[4];

	//read in pos and norm info from srv
	float4 tempPos = buffIn[index].pos;
	float3 tempNorm = buffIn[index].norm;

	//convert pos and norm data into world space
	tempPos = mul(tempPos, world);
	tempNorm = mul(tempNorm, world);

	//build quad around view space position
	verts[0].pos = tempPos;

	verts[1].pos = tempPos;
	verts[1].pos.y = tempPos.y - 0.5f;

	verts[2].pos = tempPos;
	verts[2].pos.x = tempPos.x - 0.5f;

	verts[3].pos = tempPos;
	verts[3].pos.x = tempPos.x - 0.5f;
	verts[3].pos.y = tempPos.y - 0.5f;

	////convert them into view space
	//tempPos = mul(tempPos, view);

	for (uint j = 0; j < 4; ++j)
	{
		verts[j].pos = mul(verts[j].pos, view);
	}
		
	

	for (uint i = 0; i < 4; ++i)
	{
		//convert view space positions into projection space
		verts[i].pos = mul(verts[i].pos, projection);
		//assign normals to each vert
		verts[i].normal = tempNorm;
	}
		
	//UVs
	verts[0].uv = float2(1, 0);
	verts[1].uv = float2(0, 0);
	verts[2].uv = float2(1, 1);
	verts[3].uv = float2(0, 1);

	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
	output.Append(verts[3]);
}