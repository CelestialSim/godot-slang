RWStructuredBuffer<float> MyDataBuffer;

[shader("compute")]
[numthreads(2, 1, 1)]
void computeMain(uint3 threadId: SV_DispatchThreadID)
{
    MyDataBuffer[threadId.x] *= 2;
}
