// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
 
// IN
#define PS_INPUT VertexToPixelLit
 
// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D<float4> Noise : register(t1);

cbuffer cbParam : register(b4)
{
    float3 lightHitPos;
    float dummy;
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    return float4(1, 1, 1, 1);
 //   float4 color;
    
	//// テクスチャーの色を取得
 //   color = diffuseMapTexture.Sample(diffuseMapSampler, float2(PSInput.uv.x, PSInput.uv.y - time));
    
 //   float noise = Noise.Sample(diffuseMapSampler, PSInput.uv).r;

 //   float width = 0.5f;

 //   if (noise < time)
 //   {
 //       discard;
 //   }

 //   float edge = 1.0 - smoothstep(time, time + width, noise);

 //   float3 edgeColor = float3(0, 0, 0);

 //   color.rgb = lerp(color.rgb, edgeColor, edge);

 //   return color;
}