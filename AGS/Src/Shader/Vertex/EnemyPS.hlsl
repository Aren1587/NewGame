#include "../Common/VertexToPixelHeader.hlsli"
#define PS_INPUT VertexToPixelLit
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
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

    float dist = distance(PSInput.worldpos, lightHitPos);

    if (lightRadius < dist || dist < notLightRadius)
    {
        color.rgb = 0.0f;
        return color;
    }

    float whitePos = (notLightRadius + lightRadius) * 0.5f;
    float halfWidth = max((lightRadius - notLightRadius) * 0.5f, 0.0001f);

    float d = abs(dist - whitePos);
    float intensity = 1.0f - saturate(d / halfWidth);
    intensity = smoothstep(0.0f, 1.0f, intensity);

    color.rgb *= intensity;
    return color;
}