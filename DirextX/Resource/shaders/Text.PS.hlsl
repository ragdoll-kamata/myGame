#include "Text.hlsli"

Texture2DArray<float32_t> gMainTexture : register(t0);
Texture2D<float32_t> gTempTexture : register(t1);

SamplerState gSmapler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
	PixelShaderOutput output;
   
    float32_t alpha;
	if (input.isTemp == 1) {
        alpha = gTempTexture.Sample(gSmapler, input.texcoord);
    } else {
        alpha = gMainTexture.Sample(gSmapler, float32_t3(input.texcoord.xy, input.layer));
    }
    output.color = float32_t4(1.0f, 1.0f, 1.0f, alpha);
    output.color *= input.color; // Apply vertex color
    if (output.color.a == 0.0f)
    {
        discard;
    }

	return output;
}