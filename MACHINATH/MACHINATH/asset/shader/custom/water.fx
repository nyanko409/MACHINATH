// transformation matrices
float4x4 World;
float4x4 View;
float4x4 Projection;

// water sampler
sampler s1 : register(s1); 

float time;


struct VS_IN 
{
	float3 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

struct VS_OUT
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};


VS_OUT VShader(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	float4x4 mat;

	mat = mul(mul(World, View), Projection);
	Out.pos = mul(float4(In.pos, 1.0f), mat);

	Out.uv = In.uv;
	Out.col = In.col;

	return Out;

    // init 
	//VS_OUT Vert = (VS_OUT)0;

    // offset current pos to world position
    //Pos = mul(Pos, World);
    //Normal = mul(Normal, World);

    // calc lighting and set diffuse
    //float3 LightVec = normalize(LightPos - Pos);
    //DiffuseColor = dot(Normal, LightVec) * DiffuseColor;
    //DiffuseColor = max(DiffuseColor, 0);
	//
    //Vert.col.rgb = DiffuseColor + AmbientColor;
	//Vert.col.rgb = 1;
    //Vert.col.a = 1.0F;

    // transform vertex from world space to screen space
    //float4x4 Transform;
    //Transform = mul(View, Projection);
    //Vert.pos = mul(Pos, Transform);
	//
    //return Vert;
}

float4 PShader(VS_OUT In) : COLOR0
{
	// uv scroll
	float2 uv = In.uv;
	uv.y -= time;
	uv.x += time * 0.5F;

	// get pixel color from texture sampler
	float4 temp = tex2D(s1, uv);

	// set alpha
	temp.a = 0.5F;

	return temp;
}

technique FirstTechnique
{
    pass FirstPass
    {
        //Lighting = TRUE;
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
