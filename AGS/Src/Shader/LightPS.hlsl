cbuffer cbParam : register(b4)
{
    float4 g_color;
}

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}