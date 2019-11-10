float4x4 World;
float4x4 View;
float4x4 Projection;

float3 LightPos;
float3 AmbientColor;
float3 DiffuseColor;

struct VertexOut
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

VertexOut VShader(float4 Pos : POSITION, float3 Normal : NORMAL)
{
	VertexOut Vert = (VertexOut)0;

	// float3 LightPos = float3(2, 2, 2);
	// float3 AmbientColor = float3(.5, .5, .5);
	// float3 DiffuseColor = float3(.5, .5, .5);

	Pos = mul(Pos, World);
	Normal = mul(Normal, World);
	float3 LightVec = normalize(LightPos - Pos);

	float3 NewDiffuseColor = DiffuseColor;
	NewDiffuseColor = dot(Normal, LightVec) * NewDiffuseColor;
	NewDiffuseColor = max(NewDiffuseColor, 0);

	Vert.Color.rgb = NewDiffuseColor + AmbientColor;

	float4x4 Transform;
	Transform = mul(View, Projection);
	Vert.Pos = mul(Pos, Transform);

	return Vert;
	
}

technique FirstTechnique
{
	pass FirstPass
	{
		Lighting = TRUE;
		ZEnable = TRUE;

		VertexShader = compile vs_2_0 VShader();
	}
}
