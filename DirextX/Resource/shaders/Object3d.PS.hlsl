#include "Object3d.hlsli"
struct Material {
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    float32_t shininess;
    int32_t isShininess;
};
struct DirectionalLight {
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};
struct Camera {
    float32_t3 worldPosition;
};

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);


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
  
    if (gMaterial.enableLighting != 0) {
        float cos;
        if (gMaterial.enableLighting == 1) {
            cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        } else {
            float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
            cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        }
        
        if (gMaterial.isShininess)
        {
            float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        
            float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
        
            float RdotE = dot(reflectLight, toEye);
        
            float specularPow = pow(saturate(RdotE), gMaterial.shininess);
        
            float32_t3 diffuse =
            gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
            float32_t3 specular =
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
            output.color.rgb = diffuse + specular;
        
            output.color.a = gMaterial.color.a * textureColor.a;
        }else{
            output.color =
            gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;

        }

    } else {
        output.color = gMaterial.color * textureColor;
    }

    return output;
}