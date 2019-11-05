float4x4 World;
float4x4 View;
float4x4 Projection;

struct VertexOut
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

VertexOut VShader(float4 Pos : POSITION)
{
	VertexOut Vert = (VertexOut)0;
	float4x4 Transform;

	Transform = mul(World, View);
	Transform = mul(Transform, Projection);
	Vert.Pos = mul(Pos, Transform);

	Vert.Color = float4(1, 1, 1, 1);

	return Vert;
}

technique FirstTechnique
{
	pass FirstPass
	{
		Lighting = FALSE;
		ZEnable = TRUE;

		VertexShader = compile vs_2_0 VShader();
	}
}