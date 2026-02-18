#include "FullScreen.hlsli"

struct Parameter
{
    float32_t Time;
};

Texture2D<float32_t4> gTexture : register(t0);
ConstantBuffer<Parameter> gParameter : register(b0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

//画面を歪ませる
float32_t2 Curve(float32_t2 uv)
{
    
    uv = (uv - 0.5f) * 2.0f;
    uv *= 1.1f;
    uv.x *= 1.0f + pow((abs(uv.y) / 5.0f), 2.0f);
    uv.y *= 1.0f + pow((abs(uv.x) / 4.0f), 2.0f);
    uv = (uv / 2.0f) + 0.5f;
    uv = uv * 0.92f + 0.04f;
    return uv;
    
}

PixelShaderOutput main(VertexShaderOutput input)
{
    
    PixelShaderOutput output;
    
    float32_t2 uv = Curve(input.texcoord);
    
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
    {
        output.color = float32_t4(0.0f, 0.0f, 0.0f, 1.0f);
        return output;
    }
    
    
    
    output.color.r = gTexture.Sample(gSampler, float32_t2(uv.x + 0.003f, uv.y)).r;
    output.color.g = gTexture.Sample(gSampler, float32_t2(uv.x + 0.000f, uv.y)).g;
    output.color.b = gTexture.Sample(gSampler, float32_t2(uv.x - 0.003f, uv.y)).b;
    
    //走査線
    output.color *= 0.9f + 0.1f * sin(10.0f * gParameter.Time + uv.y * 1000.0f);
    
    //走査線の強度
    output.color *= 0.99f + 0.1f * sin(110.0f * gParameter.Time + uv.y * 1000.0f);
    
    float vig = 16.0f * uv.x * uv.y * (1.0f - uv.x) * (1.0f - uv.y);
    output.color *= pow(vig, 0.3f);
    
    output.color.a = 1.0f;
    return output;
}