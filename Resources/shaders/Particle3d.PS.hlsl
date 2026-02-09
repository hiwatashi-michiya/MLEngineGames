#include "Particle3d.hlsli"
#include "Material3d.hlsli"

struct DirectionalLight {
	float32_t4 color;
	float32_t3 direction;
	float intensity;
};

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

Texture2D<float32_t4> gTexture : register(t0);
StructuredBuffer<Material> gMaterial : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
    uint32_t instanceID = input.texcoord.z;
	float4 transformedUV = mul(float32_t4(input.texcoord.xy, 0.0f, 1.0f), gMaterial[instanceID].uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
	
	//output.colorのα値が0のときにPixelを棄却
    if (output.color.a <= 0.1)
    {
        discard;
    }
	
    output.color = gMaterial[instanceID].color * textureColor * input.color;
	
	return output;
}
