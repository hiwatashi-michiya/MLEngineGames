
struct VertexShaderOutput
{
    float32_t4 position : SV_POSITION;
    //xy : uv, z : instanceID
    float32_t3 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
    float32_t3 worldPosition : POSITION0;
    float32_t4 color : COLOR0;
};

struct InstancingForGPU
{
    float32_t4x4 WVP;
    float32_t4x4 World;
    float32_t4x4 WorldInverseTranspose;
    float32_t4 color;
};
