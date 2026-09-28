// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float3 lightHitPos;
    float lightRadius;
    
    float notLightRadius;
    float maxLightRadiusSize;
    float2 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

    // 着地点からの距離
    float dist = distance(PSInput.worldpos, lightHitPos);
    
    // 最大より大きい or 最小より小さい
    if (lightRadius < dist || dist < notLightRadius)
    {
        color.rgb = 0.0f;
        return color;
    }
    
    // 白の位置
    float whitePos = (notLightRadius + lightRadius) * 0.5f;

    // 白の位置からの距離
    float d = abs(dist - whitePos);

    // 半分の幅
    float halfWidth = (lightRadius - notLightRadius) * 0.5f;

    // 黒→白→黒 (0～1～0)
    float intensity = 1.0f - saturate(d / halfWidth);

    // 滑らかに
    intensity = smoothstep(0.0f, 1.0f, intensity);

    color.rgb *= intensity;
    
    return color;
}