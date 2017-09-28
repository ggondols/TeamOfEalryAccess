#include "MultiAnimation.vsh"



float4x4	matLightView;
float4x4	gLightProjectionMatrix;



//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------

texture ShadowMap_Tex : RenderColorTarget
<
	float2 RenderTargetDimensions = { 2048,2048 };
	string Format = "D3DFMT_R32F";
	float  ClearDepth = 1.000000;
	int    ClearColor = -1;
>;

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4  Pos         : POSITION;
	float3  BlendWeights: BLENDWEIGHT;
	float4  BlendIndices: BLENDINDICES;
	float3  Normal      : NORMAL;
	float3  TexCoord    : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 mClipPosition : TEXCOORD1;
};

float4 PixScene( 
	float4 mClipPosition : TEXCOORD1):COLOR0
{
	float depth = (mClipPosition.z / mClipPosition.w);
	return float4(depth, depth, depth, 1.0f);

}


VS_OUTPUT VertSkinning( VS_INPUT Input, uniform int nNumBones )
{
	VS_OUTPUT   Output;
	

	float3	Pos			= 0.0f;
	float3	Normal		= 0.0f;
	float	LastWeight	= 0.0f;
	float4	WorldPos;
	// skin VB inputs
	VS_SKIN_INPUT vsi = { Input.Pos, Input.BlendWeights, Input.BlendIndices, Input.Normal };
	VS_SKIN_OUTPUT vso = VS_Skin( vsi, nNumBones );

	// transform position from world space into view and then projection space
	WorldPos = float4(vso.vPos.xyz, 1.0f);
	
	Output.mClipPosition = mul(WorldPos, matLightView);
	Output.mClipPosition = mul(Output.mClipPosition, gLightProjectionMatrix);
	Output.Pos = Output.mClipPosition;
	


	return Output;
}


int CurNumBones = 10;

VertexShader vsArray20[ 4 ] = { 
	compile vs_2_0 VertSkinning( 1 ),
	compile vs_2_0 VertSkinning( 2 ),
	compile vs_2_0 VertSkinning( 3 ),
	compile vs_2_0 VertSkinning( 4 ) };


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique Skinning20
{
	pass p0
	{
		VertexShader = ( vsArray20[ CurNumBones ] );
		PixelShader = compile ps_2_0 PixScene();
	}
}
