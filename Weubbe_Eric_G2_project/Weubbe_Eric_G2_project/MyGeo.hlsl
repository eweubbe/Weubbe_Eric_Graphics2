struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(3)]
void main(point float4 _input[1]: SV_POSITION, inout TriangleStream < GSOutput > output)
{
	GSOutput verts[3] =
	{
		float4(1,0,0,1),
		float4(0,1,0,1),
		float4(0,0,1,1)
	};

	verts[0].pos = _input[0];
	
	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
}