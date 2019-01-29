struct Particle
{
	float4 pos;
	float3 norm;
};

RWStructuredBuffer<Particle> outBuff : register(u0);

[numthreads(10, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	outBuff[DTid.x].pos.x += 0.001f;
	if (outBuff[DTid.x].pos.x >= 30.0f)
		outBuff[DTid.x].pos.x = -30.0f;
}