float4x4 World;
float4x4 View;
float4x4 Projection;

float3 LightPos;
float3 AmbientColor;
float3 DiffuseColor;
float3 ViewDir;

struct VertexOut
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};

VertexOut VShader(float4 Pos : POSITION, float3 Normal : NORMAL)
{
    // init 
    VertexOut Vert = (VertexOut)0;
    float3 LightPos = float3(2, 2, 2);
    float3 AmbientColor = float3(.5, .5, .5);
    float3 DiffuseColor = float3(.5, .5, .5);

    // offset current pos and normal to world position
    Pos = mul(Pos, World);
    Normal = mul(Normal, World);

    // calc lighting and set diffuse
    float3 LightVec = normalize(LightPos - Pos);
    DiffuseColor = dot(Normal, LightVec) * DiffuseColor;
    DiffuseColor = max(DiffuseColor, 0);

    Vert.Color.rgb = DiffuseColor + AmbientColor;
    Vert.Color.a = 1.0F;

    // rim lighting
    float rim = 1.0 - abs(dot(Normal, ViewDir));
    Vert.Color.a = pow(rim, 5);
    Vert.Color.g = pow(rim, 5) * 10;
    Vert.Color.rb = 0;
    

    // transform vertex from world space to screen space
    float4x4 Transform;
    Transform = mul(View, Projection);
    Vert.Pos = mul(Pos, Transform);

    return Vert;
}

float4 PShader(	float4 Diff : COLOR0, float Vfa : VFACE) : COLOR
{
    //float alpha = normal < 0.1F ? 
    //float green = normal < 0.1F && normal > -0.1F ? 1.0F : 0.0F;
    float green = 1.0F;

    return Diff;
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
