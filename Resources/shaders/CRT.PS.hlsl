#include "FullScreen.hlsli"

struct Parameter
{
    float32_t Time;
    float32_t uvSub;
    float32_t uvSubPow;
    float32_t uvPow;
    
    float32_t uvXDiv;
    float32_t uvYDiv;
    float32_t uvXYPow;
    float32_t uvDiv;
    
    float32_t uvDivAdd;
    float32_t uvEndPow;
    float32_t uvEndAdd;
    float32_t colorGap;
    
    float32_t2 uvMinRange;
    float32_t2 uvMaxRange;
    
    float32_t scanLine;
    float32_t scanLinePow;
    float32_t vigPow;
    float32_t vigEndPow;
    
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
    
    uv = (uv - gParameter.uvSub) * gParameter.uvSubPow; //uvSubtract, uvSubPower
    uv *= gParameter.uvPow; //uvPower
    uv.x *= 1.0f + pow((abs(uv.y) / gParameter.uvXDiv), gParameter.uvXYPow); //uvXDiv, uvXYPower
    uv.y *= 1.0f + pow((abs(uv.x) / gParameter.uvYDiv), gParameter.uvXYPow); //uvYDiv, uvXYPower
    uv = (uv / gParameter.uvDiv) + gParameter.uvDivAdd; //uvDivide, uvDivideAdd
    uv = uv * gParameter.uvEndPow + gParameter.uvEndAdd; //uvEndPower, uvEndAdd
    return uv;
    
}

PixelShaderOutput main(VertexShaderOutput input)
{
    
    PixelShaderOutput output;
    
    float32_t2 uv = Curve(input.texcoord);
    
    if (uv.x < gParameter.uvMinRange.x || uv.x > gParameter.uvMaxRange.x ||
        uv.y < gParameter.uvMinRange.y || uv.y > gParameter.uvMaxRange.y) //uvMinRange, uvMaxRange
    {
        output.color = float32_t4(0.0f, 0.0f, 0.0f, 1.0f);
        return output;
    }
    
    
    
    output.color.r = gTexture.Sample(gSampler, float32_t2(uv.x +gParameter.colorGap, uv.y)).r; //colorGap
    output.color.g = gTexture.Sample(gSampler, float32_t2(uv.x + 0.000f, uv.y)).g;
    output.color.b = gTexture.Sample(gSampler, float32_t2(uv.x - gParameter.colorGap, uv.y)).b;
    
    //走査線
    output.color *= gParameter.scanLine + 0.1f * sin(10.0f * gParameter.Time + uv.y * 1000.0f); //scanLine
    
    //走査線の強度
    output.color *= gParameter.scanLinePow + 0.1f * sin(110.0f * gParameter.Time + uv.y * 1000.0f); //scanLinePower
    
    float vig = gParameter.vigPow * uv.x * uv.y * (1.0f - uv.x) * (1.0f - uv.y); //vigPower
    output.color *= pow(vig, gParameter.vigEndPow); //vigEndPower
    
    output.color.a = 1.0f;
    return output;
}