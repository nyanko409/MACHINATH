float4x4 World;
float4x4 View;
float4x4 Projection;

float3 LightPos;
float3 AmbientColor;
float3 DiffuseColor;
float3 ViewDir;

sampler s0 : register(s0);
sampler s1 : register(s1);
float value;


struct VS_IN
{
	float3 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

struct VertexOut
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

VertexOut VShader(float4 Pos : POSITION, float3 Normal : NORMAL, float3 Uv : TEXCOORD0)
{
    // init 
    VertexOut Vert = (VertexOut)0;
    float3 LightPos = float3(2, 2, 2);
    float3 AmbientColor = float3(1, 1, 1);
    float3 DiffuseColor = float3(1, 1, 1);

    // offset current pos and normal to world position
    Pos = mul(Pos, World);
    Normal = mul(Normal, World);

    // calc lighting and set diffuse
    float3 LightVec = normalize(LightPos - Pos);
    DiffuseColor = dot(Normal, LightVec) * DiffuseColor;
    DiffuseColor = max(DiffuseColor, 0);

    Vert.col.rgb = DiffuseColor + AmbientColor;
    Vert.col.a = 1.0F;

    // transform vertex from world space to screen space
    float4x4 Transform;
    Transform = mul(View, Projection);
    Vert.pos = mul(Pos, Transform);

	Vert.uv = Uv;

    return Vert;
}

float4 PShader(VertexOut In) : COLOR0
{
	// get pixel color from dissolve sampler
	float4 temp = tex2D(s1, In.uv);
	
	if (temp.r < value + 0.2F)
	{
		In.col.g *= 1 - temp.r;
		In.col.rb *= temp.r;
		In.col.a = temp.r < value ? 0 : In.col.a;
	}

	// get pixel color from diffuse sampler
	float4 color = tex2D(s0, In.uv);
	In.col *= color;

    return In.col;
}

technique FirstTechnique
{
    pass FirstPass
    {
        Lighting = TRUE;
        ZEnable = TRUE;

        NormalizeNormals = TRUE;

        // texture stages
		ColorOp[0] = MODULATE;
		ColorArg1[0] = TEXTURE;
		ColorArg2[0] = DIFFUSE;
		AlphaOp[0] = MODULATE;
		AlphaArg1[0] = TEXTURE;
		AlphaArg2[0] = DIFFUSE;

        AlphaBlendEnable = TRUE;

        VertexShader = compile vs_2_0 VShader();
        PixelShader = compile ps_2_0 PShader();
    }
}
