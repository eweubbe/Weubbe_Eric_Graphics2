//end of day 2 videos for hlsl primer

//rule of three
// three things must match (DATA)
// 1) C++ Vertex struct 
// 2) input layout
// 3) HLSL Vertex struct

struct InputVertex
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

struct OutputVertex
{
	float4 pos : SV_POSITION; //SV: system value
	float4 color : COLOR;
	float2 uv : TEXCOORD0;
};

OutputVertex main(InputVertex _input)
{
	OutputVertex output = (OutputVertex)0;
	output.pos = _input.pos;

	//Do math here

	return output;
}