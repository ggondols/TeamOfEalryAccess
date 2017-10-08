//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// ApplyShadowShader
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// ApplyShadowTorus
//--------------------------------------------------------------//
string ApplyShadowShader_ApplyShadowTorus_Torus : ModelData = ".\\Torus.x";

struct VS_INPUT 
{
   float4 mPosition: POSITION;
   float3 mNormal: NORMAL;
   float2 mText:    TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 mPosition: POSITION;
   float4 mClipPosition: TEXCOORD1;
   float mDiffuse: TEXCOORD2;
   float2 mText:    TEXCOORD0;
};

float4x4 matWorld : World;
float4x4 matLightView;

float4x4 matLightProjection : Projection;

float4 m_vec4LightPosition;

float4x4 matViewProjection : ViewProjection;

VS_OUTPUT ApplyShadowShader_ApplyShadowTorus_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
  
   float4x4 lightViewMatrix = matLightView;


   float4 worldPosition = mul(Input.mPosition, matWorld);
   Output.mPosition = mul(worldPosition, matViewProjection);

   Output.mClipPosition = worldPosition;// mul(worldPosition, matWorld);
   Output.mClipPosition = mul(Output.mClipPosition, lightViewMatrix);
   Output.mClipPosition = mul(Output.mClipPosition, matLightProjection);
   
   float3 lightDir = normalize(worldPosition.xyz - m_vec4LightPosition.xyz);
   float3 worldNormal = normalize(mul(Input.mNormal, (float3x3)matWorld));
   Output.mDiffuse = dot(-lightDir, worldNormal);
   Output.mText =Input.mText;

   return Output;
}
texture ShadowMap_Tex
<
   string ResourceName = ".\\";
>;
sampler2D ShadowSampler = sampler_state
{
   Texture = (ShadowMap_Tex);
};

texture heightMap_Tex
<
	string ResourceName = ".\\";
>;
sampler2D heightSampler = sampler_state
{
	Texture = (heightMap_Tex);
};
float4 gObjectColor;
struct PS_INPUT
{
   float4 mClipPosition: TEXCOORD1;
   float mDiffuse: TEXCOORD2;
   float2 mText:    TEXCOORD0;

};

float4 ApplyShadowShader_ApplyShadowTorus_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   float3 rgb = (Input.mDiffuse);
   
   float currentDepth = Input.mClipPosition.z / Input.mClipPosition.w;
   
   float2 uv = Input.mClipPosition.xy / Input.mClipPosition.w;

   uv.y = -uv.y;
   uv = saturate(uv *0.5+0.5);
 
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;
   
   if (currentDepth > shadowDepth + 0.0000125f)
   {
      rgb *= 0.5f;
   }
   else
   {
	   rgb = (tex2D(heightSampler ,Input.mText).xyz);
   }
   return(float4( rgb, 1.0f ) );
}




//--------------------------------------------------------------//
// Technique Section for ApplyShadowShader
//--------------------------------------------------------------//
technique ApplyShadowShader
{
   pass ApplyShadowTorus
   {
      VertexShader = compile vs_2_0 ApplyShadowShader_ApplyShadowTorus_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ApplyShadowShader_ApplyShadowTorus_Pixel_Shader_ps_main();
   }

}

