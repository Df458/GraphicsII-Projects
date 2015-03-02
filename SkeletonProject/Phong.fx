uniform extern float4x4 worldViewProjection;
uniform extern float4x4 worldMatrix;
uniform extern float4 vectorLightDirection;
uniform extern float4 cameraPos;

struct OutputVS //vertex shader output
{
	float4 pos		: POSITION0;
	float3 light	: TEXTCOORD0;
	float3 normal	: TEXTCOORD1;
	float3 view		: TEXTCOORD2;
};

OutputVS PhongVS(float4 pos : POSITION0, float3 normal : NORMAL0)
{
	OutputVS outVS = (OutputVS)0; //Set inital output to 0

	outVS.pos = mul(pos, worldViewProjection);//Transforms normals to world space
	outVS.light = vectorLightDirection; // Sets the light vector
	outVS.view = cameraPos - normalize(mul(pos, worldMatrix));// Sets the view vector
	outVS.normal = mul(normal, worldMatrix);// Sets the normal vector

	return outVS;
}

float4 PhongPS(float3 Light: TEXCOORD0, float3 Norm : TEXCOORD1, float3 View : TEXCOORD2) : COLOR
{
	float4 AmbientColor = { 0.1f, 0.0f, 0.0f, 1.0f };
	float4 DiffuseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	float3 Normal = normalize(Norm);
	float3 LightDirection = normalize(Light);
	float3 ViewDirection = normalize(View);
	float4 Diffuse = saturate(dot(Normal, LightDirection));

	float4 Shadow = saturate(4 * Diffuse);

	float3 Reflect = normalize(2 * Diffuse * Normal - LightDirection); // Calculate reflections
	float4 Specular = pow(saturate(dot(Reflect, ViewDirection)), 8); // Calculate Specular (Relections.ViewDirection)^8
	return AmbientColor + Shadow * (DiffuseColor * Diffuse + Specular);

}