VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;

    float4 lLocalPosition = float4(VSInput.pos, 1.0f);
    float4 lWorldPosition;
    float4 lViewPosition;

    // ボーン行列をウェイトでブレンド
    float4 m0 = 0.0f;
    float4 m1 = 0.0f;
    float4 m2 = 0.0f;

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        int idx = VSInput.blendIndices0[i];
        float w = VSInput.blendWeight0[i];
        m0 += g_localWorldMatrix.lwMatrix[idx + 0] * w;
        m1 += g_localWorldMatrix.lwMatrix[idx + 1] * w;
        m2 += g_localWorldMatrix.lwMatrix[idx + 2] * w;
    }
#if VERTEX_INPUT == DX_MV1_VERTEX_TYPE_8FRAME || VERTEX_INPUT == DX_MV1_VERTEX_TYPE_NMAP_8FRAME
    [unroll]
    for (int j = 0; j < 4; j++)
    {
        int idx = VSInput.blendIndices1[j];
        float w = VSInput.blendWeight1[j];
        m0 += g_localWorldMatrix.lwMatrix[idx + 0] * w;
        m1 += g_localWorldMatrix.lwMatrix[idx + 1] * w;
        m2 += g_localWorldMatrix.lwMatrix[idx + 2] * w;
    }
#endif

    lWorldPosition.xyz = float3(dot(lLocalPosition, m0),
                                dot(lLocalPosition, m1),
                                dot(lLocalPosition, m2));
    lWorldPosition.w = 1.0f;

    float4 n = float4(VSInput.norm, 0.0f);
    ret.normal = normalize(float3(dot(n, m0), dot(n, m1), dot(n, m2)));

    ret.worldpos.xyz = lWorldPosition.xyz;

    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;
    ret.diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
    ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);

    return ret;
}