#include "InstancingObject3d.hlsli"
#include "Material3d.hlsli"

struct InstancingForGPU
{
    float32_t4x4 WVP;
    float32_t4x4 World;
    float32_t4x4 WorldInverseTranspose;
    float32_t4 color;
};

StructuredBuffer<InstancingForGPU> gInstancing : register(t0);
//use for normalMap
Texture2D<float32_t4> gTexture : register(t1);
SamplerState gSampler : register(s0);
ConstantBuffer<MaterialOptions> gMaterialOptions : register(b0);
ConstantBuffer<Material> gMaterial : register(b1);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 nMap = gTexture.Sample(gSampler, transformedUV.xy) * 2 - 1;
    
    output.position = mul(input.position, gInstancing[instanceId].WVP);
    output.texcoord = input.texcoord;
    
    //normal map
    if (gMaterialOptions.enableNormalMap != 0)
    {
        output.normal = normalize(mul(normalize(nMap.xyz), (float32_t3x3) gInstancing[instanceId].WorldInverseTranspose));
    }
    //default
    else
    {
        output.normal = normalize(mul(input.normal, (float32_t3x3) gInstancing[instanceId].WorldInverseTranspose));
    }
    
    output.worldPosition = mul(input.position, gInstancing[instanceId].World).xyz;
    output.color = gInstancing[instanceId].color;
    return output;
}
