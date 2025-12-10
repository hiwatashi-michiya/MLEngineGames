#include "Object3d.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
	float32_t4x4 World;
    float32_t4x4 WorldInverseTranspose;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
//use for normalMap
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	
    float32_t4 nMap = gTexture.Sample(gSampler, input.texcoord) * 2 - 1;
	
	output.position = mul(input.position, gTransformationMatrix.WVP);
	output.texcoord = input.texcoord;
    output.normal = normalize(mul(normalize(nMap.xyz), (float32_t3x3) gTransformationMatrix.WorldInverseTranspose));
    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
	return output;
}
