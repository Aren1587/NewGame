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
    float3 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);

    // 着地点からの距離
    float dist = distance(PSInput.worldpos, lightHitPos);

    if (dist < lightRadius)
    {
        if(dist < notLightRadius)
        {    
            // それ以外は暗く
            color.rgb *= 0.05f;
        }
        else
        {
           // 半径内だけ白
            color.rgb *= float3(1.0, 1.0, 1.0);
        }
    }
    else
    {
        // それ以外は暗く
        color.rgb *= 0.05f;
    }

    return color;
}