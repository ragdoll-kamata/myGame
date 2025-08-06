#include "Object3d.hlsli"
struct Material {
    float32_t4 color;
    float32_t4x4 uvTransform;
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSmapler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput {
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSmapler, transformedUV.xy);
    output.color = gMaterial.color * textureColor;
    return output;

}