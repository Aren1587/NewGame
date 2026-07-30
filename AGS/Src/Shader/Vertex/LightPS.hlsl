// VS/PS‹¤’Ê
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

cbuffer cbParam : register(b4)
{
    float3 lightHitPos;
    float lightRadius;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

    // ’…’n“_‚©‚ç‚Ì‹——£
    float dist = distance(PSInput.worldpos, lightHitPos);

    if (dist < lightRadius)
    {
        if (dist < lightRadius)
        {
            // ”¼Œa“à‚¾‚¯”’
            color.rgb = float3(1.0, 1.0, 1.0);
        }
        else
        {
            color.rgb *= 0.05;
        }

    }
    else
    {
    // ‚»‚êˆÈŠO‚ÍˆÃ‚­
        color.rgb *= 0.05;
    }

    return color;
}