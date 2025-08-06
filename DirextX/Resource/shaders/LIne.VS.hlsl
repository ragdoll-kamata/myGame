#include "Line.hlsli"

struct TransformationMatrix
{
    float32_t4x4 startWVP;
    float32_t4x4 endWVP;
    float32_t4 color;
};
StructuredBuffer<TransformationMatrix> gTransformationMatrix : register(t0);


VertexShaderOutput main(uint vertexID : SV_VertexID) {
    uint32_t index = vertexID / 2;
    
    bool isStartVertex = (vertexID % 2) == 0;
    
    VertexShaderOutput output;
    if (isStartVertex)
    {
        output.position = mul(float32_t4(0.0f, 0.0f, 0.0f, 1.0f), gTransformationMatrix[index].startWVP);
    }
    else
    {
        output.position = mul(float32_t4(0.0f, 0.0f, 0.0f, 1.0f), gTransformationMatrix[index].endWVP);
    }
    output.color = gTransformationMatrix[index].color;
    return output;
}