struct Particle
{
	float4 pos;
	float3 norm;
};

StructuredBuffer<Particle> inBuff : register(t0);
RWStructuredBuffer<Particle> outBuff : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{

}