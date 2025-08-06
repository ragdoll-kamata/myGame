#include "Text.hlsli"

struct TextData {
	float32_t4x4 WVP; // 行列（行優先／列優先は要注意）
    float32_t4 color; // 色（RGBA）
	float32_t2 uvTopLeft;
	float32_t2 uvBottomRight;
	uint32_t layerIndex;
	uint32_t isTemp;
};

StructuredBuffer<TextData> gTextData : register(t0);


struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID) {
	VertexShaderOutput output;
	output.position = mul(input.position, gTextData[instanceId].WVP);
    output.color = gTextData[instanceId].color;
	output.texcoord = lerp(gTextData[instanceId].uvTopLeft, gTextData[instanceId].uvBottomRight, input.texcoord);
	output.layer = gTextData[instanceId].layerIndex;
	output.isTemp = gTextData[instanceId].isTemp;
	return output;
}