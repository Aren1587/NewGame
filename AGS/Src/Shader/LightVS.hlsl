cbuffer cbParam : register(b4)
{
    float4 g_color;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}