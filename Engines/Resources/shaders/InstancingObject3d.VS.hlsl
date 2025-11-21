#include "InstancingObject3d.hlsli"

StructuredBuffer<InstancingForGPU> gInstancing : register(t0);

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    
    output.position = mul(input.position, gInstancing[instanceId].WVP);
    output.texcoord.xy = input.texcoord;
    output.texcoord.z = instanceId;
    output.normal = normalize(mul(input.normal, (float32_t3x3) gInstancing[instanceId].WorldInverseTranspose));
    output.worldPosition = mul(input.position, gInstancing[instanceId].World).xyz;
    output.color = gInstancing[instanceId].color;
    return output;
}
