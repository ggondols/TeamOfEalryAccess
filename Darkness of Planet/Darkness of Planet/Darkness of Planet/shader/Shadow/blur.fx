//======================================================================
//
//      HIGH DYNAMIC RANGE RENDERING DEMONSTRATION
//      Written by Jack Hoxley, November 2005
//
//======================================================================

//------------------------------------------------------------------
//  GLOBAL VARIABLES
//------------------------------------------------------------------
float HBloomWeights[9];                 // Description of the sampling distribution used by
float HBloomOffsets[9];                 // the HorizontalBlur() function

float VBloomWeights[9];                 // Description of the sampling distribution used by
float VBloomOffsets[9];                 // the VerticalBlur() function

texture		TempRenderTargetTexture;
sampler2D TextureSampler = sampler_state
{
   Texture = (TempRenderTargetTexture);
};

//------------------------------------------------------------------
// HORIZONTAL BLUR
//
// Takes 9 samples from the down-sampled texture (4 either side and
// one central) biased by the provided weights. Different weight
// distributions will give more subtle/pronounced blurring.
//------------------------------------------------------------------
float4 HorizontalBlur( in float2 t : TEXCOORD0 ) : COLOR
{
    float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for( int i = 0; i < 9; i++ )
    {
        color += (tex2D( TextureSampler, t + float2( HBloomOffsets[i], 0.0f ) ) * HBloomWeights[i] );
    }
        
    return float4( color.rgb, 1.0f );
}
    
//------------------------------------------------------------------
// VERTICAL BLUR
//
// Takes 9 samples from the down-sampled texture (4 above/below and
// one central) biased by the provided weights. Different weight
// distributions will give more subtle/pronounced blurring.
//------------------------------------------------------------------
float4 VerticalBlur( in float2 t : TEXCOORD0 ) : COLOR
{
    float4 color = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for( int i = 0; i < 9; i++ )
    {
        color += (tex2D( TextureSampler, t + float2( 0.0f, VBloomOffsets[i] ) ) * VBloomWeights[i] );
    }
        
    return float4( color.rgb, 1.0f );
}

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = Input;
   return( Output );
}

float4 ps_main(float2 TexCoord : TEXCOORD0) : COLOR0
{     
   return( (HorizontalBlur(TexCoord) + VerticalBlur(TexCoord)) / 2 );
}

//--------------------------------------------------------------//
// Technique Section for Textured Phong
//--------------------------------------------------------------//
technique Blur
{
   pass Pass0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}

