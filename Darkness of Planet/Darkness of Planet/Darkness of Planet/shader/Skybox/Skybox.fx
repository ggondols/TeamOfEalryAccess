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
// Default_DirectX_Effect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Default_DirectX_Effect_Pass_0_Model : ModelData = ".\\";

float4x4 matViewProjection : ViewProjection;
float4 vViewPosition : ViewPosition;


struct VS_INPUT 
{
   float4 Position : POSITION0;
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Position3D : TEXCOORD0;
   
};

float4x4 translation(float3 position)
{
   return float4x4(
      float4(1,0,0,0),
      float4(0,1,0,0),
      float4(0,0,1,0),
      float4(position.xyz,1)
      );
}

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
   float4x4 world = translation(vViewPosition.xyz);
   
   float4x4 WorldViewProjection = mul(world,matViewProjection);

   Output.Position = mul( Input.Position, matViewProjection );
   
   Output.Position3D = Input.Position;
   
   
   
   return( Output );
   
}




texture myCubemap_Tex
<
   string ResourceName = "..\\..\\..\\
>;
sampler Texture = sampler_state
{
   Texture = (myCubemap_Tex);
};

struct PS_OUTPUT
{
   float4 position : POSITION0;
   float3 position3D : TEXCOORD0;
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_OUTPUT Input) : COLOR0
{   
   return texCUBE(Texture, Input.position3D);
   
}




//--------------------------------------------------------------//
// Pass 1
//--------------------------------------------------------------//
string Default_DirectX_Effect_Pass_1_Model : ModelData = ".\\";

struct Default_DirectX_Effect_Pass_1_Vertex_Shader_VS_INPUT 
{
   float4 Position : POSITION0;
   
};

struct Default_DirectX_Effect_Pass_1_Vertex_Shader_VS_OUTPUT 
{
   float4 Position : POSITION0;
   
};

Default_DirectX_Effect_Pass_1_Vertex_Shader_VS_OUTPUT Default_DirectX_Effect_Pass_1_Vertex_Shader_vs_main( Default_DirectX_Effect_Pass_1_Vertex_Shader_VS_INPUT Input )
{
   Default_DirectX_Effect_Pass_1_Vertex_Shader_VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matViewProjection);
   
   return( Output );
   
}




float4 Default_DirectX_Effect_Pass_1_Pixel_Shader_ps_main() : COLOR0
{   
   return( float4( 1.0f, 0.0f, 0.0f, 1.0f ) );
   
}




//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      ZWRITEENABLE = FALSE;
      CULLMODE = CW;

      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

   pass Pass_1
   {
      CULLMODE = CCW;
      ZWRITEENABLE = TRUE;

      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_1_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_1_Pixel_Shader_ps_main();
   }

}

