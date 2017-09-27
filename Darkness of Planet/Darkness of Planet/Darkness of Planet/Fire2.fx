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
// Fire Effects
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// FireBall
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Fire_Effects_FireBall_Single_Pass_ScreenAlignedQuad : ModelData = "D:\\RenderMonkey\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float time_0_X : Time0_X;
float4x4 matWorldViewProjectionMatrix;
float timeSampleDist
<
   string UIName = "timeSampleDist";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.30 );

struct VS_OUTPUT 
{
   float4 Pos:   POSITION;
   float2 pos:   TEXCOORD0;
   float2 fPos:  TEXCOORD1;
   float2 pPos:  TEXCOORD2;
   float2 ppPos: TEXCOORD3;
};

VS_OUTPUT Fire_Effects_FireBall_Single_Pass_Vertex_Shader_main(float4 Pos: POSITION)
{
   VS_OUTPUT Out;
   Out.Pos = mul(Pos, matWorldViewProjectionMatrix);
   // Clean up inaccuracies
   Pos.xy = sign(Pos.xy);

  // Out.Pos = float4(Pos.xy, 0, 1);
   Out.pos = Pos.xy;
   
   
   // Current fire ball position
   Out.fPos.x = 0.8 * sin(0.71 * time_0_X);
   Out.fPos.y = 0.8 * cos(0.93 * time_0_X);

   // Fire ball position not too long ago
   time_0_X -= timeSampleDist;
   Out.pPos.x = 0.8 * sin(0.71 * time_0_X);
   Out.pPos.y = 0.8 * cos(0.93 * time_0_X);

   // Fire ball position some time ago
   time_0_X -= timeSampleDist;
   Out.ppPos.x = 0.8 * sin(0.71 * time_0_X);
   Out.ppPos.y = 0.8 * cos(0.93 * time_0_X);

   return Out;
}


float colorDistribution
<
   string UIName = "colorDistribution";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 4.00;
> = float( 2.76 );
float fade
<
   string UIName = "fade";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 3.00;
> = float( 2.40 );
float flameSpeed
<
   string UIName = "flameSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.22 );
float spread
<
   string UIName = "spread";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 2.00;
> = float( 0.50 );
float Fire_Effects_FireBall_Single_Pass_Pixel_Shader_time_0_X : Time0_X;
float flamability
<
   string UIName = "flamability";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 3.00;
> = float( 1.74 );

texture Flame_Tex
<
   string ResourceName = "D:\\RenderMonkey\\Examples\\Media\\Textures\\Flame.tga";
>;
sampler Flame = sampler_state
{
   Texture = (Flame_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture Noise_Tex
<
   string ResourceName = "D:\\RenderMonkey\\Examples\\Media\\Textures\\NoiseVolume.dds";
>;
sampler Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

float4 Fire_Effects_FireBall_Single_Pass_Pixel_Shader_main( float2 pos:   TEXCOORD0, 
             float2 fPos:  TEXCOORD1, 
             float2 pPos:  TEXCOORD2, 
             float2 ppPos: TEXCOORD3 ) 
  : COLOR 
{
   // Distance to three points on the path, streches fire ball along the path
   float dist = distance(pos, fPos) + 0.7 * distance(pos, pPos) + 0.5 * distance(pos, ppPos);

   // Grab some noise and make a flame
   float noisy = tex3D(Noise, float3(pos, spread * dist - flameSpeed * Fire_Effects_FireBall_Single_Pass_Pixel_Shader_time_0_X)).r;
   float flame = saturate(1 - fade * dist + flamability * noisy);

   // Map flame into a color
   return tex1D(Flame, pow(flame, colorDistribution));
}


//--------------------------------------------------------------//
// Explosion
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Fire_Effects_Explosion_Single_Pass_ScreenAlignedQuad : ModelData = "D:\\RenderMonkey\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

struct Fire_Effects_Explosion_Single_Pass_Vertex_Shader_VS_OUTPUT 
{
   float4 Pos: POSITION;
   float2 texCoord: TEXCOORD0;
};

Fire_Effects_Explosion_Single_Pass_Vertex_Shader_VS_OUTPUT Fire_Effects_Explosion_Single_Pass_Vertex_Shader_main(float4 Pos: POSITION)
{
   Fire_Effects_Explosion_Single_Pass_Vertex_Shader_VS_OUTPUT Out;

   // Clean up inaccuracies
   Pos.xy = sign(Pos.xy);

   Out.Pos = float4(Pos.xy, 0, 1);
   Out.texCoord = Pos.xy;

   return Out;
}





float Fire_Effects_Explosion_Single_Pass_Pixel_Shader_flamability
<
   string UIName = "Fire_Effects_Explosion_Single_Pass_Pixel_Shader_flamability";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.34 );
float pressure
<
   string UIName = "pressure";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.60 );
float powerBoost
<
   string UIName = "powerBoost";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -2.00;
   float UIMax = 2.00;
> = float( 0.16 );
float intensity
<
   string UIName = "intensity";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 2.00;
> = float( 1.00 );
float speed
<
   string UIName = "speed";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.15 );
float noisiness
<
   string UIName = "noisiness";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.50 );
float Fire_Effects_Explosion_Single_Pass_Pixel_Shader_time_0_X : Time0_X;
float explositivity
<
   string UIName = "explositivity";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.45 );

sampler Fire_Effects_Explosion_Single_Pass_Pixel_Shader_Flame = sampler_state
{
   Texture = (Flame_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
sampler Fire_Effects_Explosion_Single_Pass_Pixel_Shader_Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

float4 Fire_Effects_Explosion_Single_Pass_Pixel_Shader_main(float2 texCoord: TEXCOORD0) : COLOR 
{
   float t = frac(Fire_Effects_Explosion_Single_Pass_Pixel_Shader_time_0_X * speed);

   // Alter the timing
   t = pow(t, explositivity);

   // The function f(t) = 6.75 * t * (t * (t - 2) + 1)
   // is a basic third degree pulse function with these properties:
   // f(0)  = 0
   // f(1)  = 0
   // f'(1) = 0
   // max(f(t)) = 1, where 0 < t < 1
   // 
   // The basic idea of this function is a quick rise at the
   // beginning and then a slow smooth decline towards zero
   float size = intensity * 6.75 * t * (t * (t - 2) + 1);

   float dist = length(texCoord) / (0.1 + size);

   // Higher Fire_Effects_Explosion_Single_Pass_Pixel_Shader_flamability => quicker move away from center
   // Higher pressure => tighter packing
   float n = tex3D(Fire_Effects_Explosion_Single_Pass_Pixel_Shader_Noise, float3(noisiness * texCoord, Fire_Effects_Explosion_Single_Pass_Pixel_Shader_flamability * Fire_Effects_Explosion_Single_Pass_Pixel_Shader_time_0_X - pressure * dist)).r;

   float4 flame = tex1D(Fire_Effects_Explosion_Single_Pass_Pixel_Shader_Flame, size * powerBoost + size * (2 * n - dist));

   return flame;
}





//--------------------------------------------------------------//
// Lava
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Fire_Effects_Lava_Single_Pass_ScreenAlignedQuad : ModelData = "D:\\RenderMonkey\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 view_proj_matrix;
struct Fire_Effects_Lava_Single_Pass_Vertex_Shader_VS_OUTPUT 
{
   float4 Pos: POSITION;
   float2 pos: TEXCOORD0;
};

Fire_Effects_Lava_Single_Pass_Vertex_Shader_VS_OUTPUT Fire_Effects_Lava_Single_Pass_Vertex_Shader_main(float4 Pos: POSITION)
{
   Fire_Effects_Lava_Single_Pass_Vertex_Shader_VS_OUTPUT Out;

   // Clean up inaccuracies   
   Pos.xy = sign(Pos.xy);

   Out.Pos = float4(Pos.xy, 0, 1);
   // Image-space
   Out.pos = 0.5 * Pos.xy;

   return Out;
}




float noiseScale
<
   string UIName = "noiseScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.21 );
float morphRate
<
   string UIName = "morphRate";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.20;
> = float( 0.05 );
float bubbleRate
<
   string UIName = "bubbleRate";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.50;
> = float( 0.12 );
float bubbleScale
<
   string UIName = "bubbleScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 5.00;
> = float( 0.95 );
float Fire_Effects_Lava_Single_Pass_Pixel_Shader_time_0_X : Time0_X;
float scale
<
   string UIName = "scale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 2.00;
> = float( 0.82 );
float bias
<
   string UIName = "bias";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( -0.12 );

sampler Fire_Effects_Lava_Single_Pass_Pixel_Shader_Flame = sampler_state
{
   Texture = (Flame_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
sampler Fire_Effects_Lava_Single_Pass_Pixel_Shader_Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

// We use noise as our base texture. Then we use more noisy to
// bubble in that noise. Both the base noise and bubble noise is
// animated, though the bubbling at a higher rate.

float4 Fire_Effects_Lava_Single_Pass_Pixel_Shader_main(float2 pos: TEXCOORD0) : COLOR 
{
   // Bubble coords
   float3 coord;
   coord.xy = pos * bubbleScale;
   coord.z = bubbleRate * Fire_Effects_Lava_Single_Pass_Pixel_Shader_time_0_X;

   // Sample noise for x and y
   float noiseX = tex3D(Fire_Effects_Lava_Single_Pass_Pixel_Shader_Noise, coord).r - 0.5;
   float noiseY = tex3D(Fire_Effects_Lava_Single_Pass_Pixel_Shader_Noise, coord + 0.5).r - 0.5;

   // Offset our base noise with the bubble noise
   coord.x = pos.x + noiseX * noiseScale;
   coord.y = pos.y + noiseY * noiseScale;
   coord.z = Fire_Effects_Lava_Single_Pass_Pixel_Shader_time_0_X * morphRate;

   // Sample the base noise
   float base = tex3D(Fire_Effects_Lava_Single_Pass_Pixel_Shader_Noise, coord).r;

   // Assign a firey color from the base noise
   return tex1D(Fire_Effects_Lava_Single_Pass_Pixel_Shader_Flame, scale * base + bias);
}


//--------------------------------------------------------------//
// Inferno
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Fire_Effects_Inferno_Single_Pass_ScreenAlignedQuad : ModelData = "D:\\RenderMonkey\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

struct Fire_Effects_Inferno_Single_Pass_Vertex_Shader_VS_OUTPUT 
{
   float4 Pos: POSITION;
   float2 texCoord: TEXCOORD0;
};

Fire_Effects_Inferno_Single_Pass_Vertex_Shader_VS_OUTPUT Fire_Effects_Inferno_Single_Pass_Vertex_Shader_main(float4 Pos: POSITION)
{
   Fire_Effects_Inferno_Single_Pass_Vertex_Shader_VS_OUTPUT Out;

   // Clean up inaccuracies
  Out.Pos  = mul(Pos, matWorldViewProjectionMatrix);

   Pos.xy = sign(Pos.xy);

   // Image-space
   Out.texCoord = Pos.xy;

   return Out;
}











float Fire_Effects_Inferno_Single_Pass_Pixel_Shader_time_0_X : Time0_X;
float sideFade
<
   string UIName = "sideFade";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 30.00;
> = float( 18.60 );
float sideFadeSharpness
<
   string UIName = "sideFadeSharpness";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.05 );
float wobbleScale
<
   string UIName = "wobbleScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.10;
> = float( 0.03 );
float burnSpeed
<
   string UIName = "burnSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.57 );
float randomnessRate
<
   string UIName = "randomnessRate";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.00;
> = float( 0.24 );
float yFade
<
   string UIName = "yFade";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 2.00;
> = float( 0.60 );
float xScale
<
   string UIName = "xScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 3.00;
> = float( 1.50 );
float yScale
<
   string UIName = "yScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 1.50;
> = float( 0.60 );

sampler Fire_Effects_Inferno_Single_Pass_Pixel_Shader_Flame = sampler_state
{
   Texture = (Flame_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
sampler Fire_Effects_Inferno_Single_Pass_Pixel_Shader_Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

float4 Fire_Effects_Inferno_Single_Pass_Pixel_Shader_main(float2 texCoord: TEXCOORD0) : COLOR 
{
   // Wobble for the noise to get a more realistic appearance
   float wobbX = 2 * cos(6 * texCoord.x + Fire_Effects_Inferno_Single_Pass_Pixel_Shader_time_0_X);
   float wobbY = 7 * (1 - texCoord.y) * (1 - texCoord.y);
   float wobble = sin(Fire_Effects_Inferno_Single_Pass_Pixel_Shader_time_0_X + wobbX + wobbY);
   // Alternative approach
   //   float wobble = 9 * (2 * tex3D(Fire_Effects_Inferno_Single_Pass_Pixel_Shader_Noise, float3(texCoord * 0.4, 0.2 * Fire_Effects_Inferno_Single_Pass_Pixel_Shader_time_0_X)) - 1);

   float3 coord;
   // Wobble more in the flames than at the base
   coord.x = xScale * texCoord.x + wobbleScale * (texCoord.y + 1) * wobble;
   // Create an upwards movement
   coord.y = yScale * texCoord.y - burnSpeed * Fire_Effects_Inferno_Single_Pass_Pixel_Shader_time_0_X;
   // Move in Z to get some randomness
   coord.z = randomnessRate * Fire_Effects_Inferno_Single_Pass_Pixel_Shader_time_0_X;
   float noisy = tex3D(Fire_Effects_Inferno_Single_Pass_Pixel_Shader_Noise, coord).r;

   // Define the shape of the fire
   float t = sideFadeSharpness * (1 - sideFade * texCoord.x * texCoord.x);

   // Get the color out of it all
   float heat = saturate(t + noisy - yFade * texCoord.y);
   float4 flame = tex1D(Fire_Effects_Inferno_Single_Pass_Pixel_Shader_Flame, heat);
if (heat<0.1)
{
	flame.w = 0;
}
   return flame;
}


//--------------------------------------------------------------//
// Fire
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Fire_Effects_Fire_Single_Pass_ScreenAlignedQuad : ModelData = "D:\\RenderMonkey\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4 layer_speed
<
   string UIName = "layer_speed";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.69, 0.52, 0.75, 1.00 );
float Fire_Effects_Fire_Single_Pass_Vertex_Shader_time_0_X : Time0_X;

struct Fire_Effects_Fire_Single_Pass_Vertex_Shader_VS_OUTPUT
{
   float4 Pos       : POSITION;
   float3 TexCoord0 : TEXCOORD0;
   float3 TexCoord1 : TEXCOORD1;
   float3 TexCoord2 : TEXCOORD2;
   float3 TexCoord3 : TEXCOORD3;
};

Fire_Effects_Fire_Single_Pass_Vertex_Shader_VS_OUTPUT Fire_Effects_Fire_Single_Pass_Vertex_Shader_main (float4 vPosition: POSITION, float3 vTexCoord0 : TEXCOORD0)
{
   Fire_Effects_Fire_Single_Pass_Vertex_Shader_VS_OUTPUT Out = (Fire_Effects_Fire_Single_Pass_Vertex_Shader_VS_OUTPUT) 0; 

   // Align quad with the screen
   Out.Pos = float4 (vPosition.x, vPosition.y, 0.0f, 1.0f);

   // Output TexCoord0 directly
   Out.TexCoord0 = vTexCoord0;

   // Base texture coordinates plus scaled time
   Out.TexCoord1.x = vTexCoord0.x;
   Out.TexCoord1.y = vTexCoord0.y + layer_speed.x * Fire_Effects_Fire_Single_Pass_Vertex_Shader_time_0_X;

   // Base texture coordinates plus scaled time
   Out.TexCoord2.x = vTexCoord0.x;
   Out.TexCoord2.y = vTexCoord0.y + layer_speed.y * Fire_Effects_Fire_Single_Pass_Vertex_Shader_time_0_X;

   // Base texture coordinates plus scaled time
   Out.TexCoord3.x = vTexCoord0.x;
   Out.TexCoord3.y = vTexCoord0.y + layer_speed.z * Fire_Effects_Fire_Single_Pass_Vertex_Shader_time_0_X;

   return Out;
}




float distortion_amount2
<
   string UIName = "distortion_amount2";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.07 );
float4 height_attenuation
<
   string UIName = "height_attenuation";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.44, 0.29, 0.00, 1.00 );
float distortion_amount1
<
   string UIName = "distortion_amount1";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.09 );
float distortion_amount0
<
   string UIName = "distortion_amount0";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.12 );
texture fire_base_Tex
<
   string ResourceName = "D:\\RenderMonkey\\Examples\\Media\\Textures\\FireBase.tga";
>;
sampler fire_base = sampler_state
{
   Texture = (fire_base_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   ADDRESSW = CLAMP;
};
texture fire_distortion_Tex
<
   string ResourceName = "D:\\RenderMonkey\\Examples\\Media\\Textures\\FireDistortion.tga";
>;
sampler fire_distortion = sampler_state
{
   Texture = (fire_distortion_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   ADDRESSW = WRAP;
};
texture fire_opacity_Tex
<
   string ResourceName = "D:\\RenderMonkey\\Examples\\Media\\Textures\\FireOpacity.tga";
>;
sampler fire_opacity = sampler_state
{
   Texture = (fire_opacity_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   ADDRESSW = CLAMP;
};

// Bias and double a value to take it from 0..1 range to -1..1 range
float4 bx2(float x)
{
   return 2.0f * x - 1.0f;
}

float4 Fire_Effects_Fire_Single_Pass_Pixel_Shader_main (float4 tc0 : TEXCOORD0, float4 tc1 : TEXCOORD1,
             float4 tc2 : TEXCOORD2, float4 tc3 : TEXCOORD3) : COLOR
{
   // Sample noise map three times with different texture coordinates
   float4 noise0 = tex2D(fire_distortion, tc1);
   float4 noise1 = tex2D(fire_distortion, tc2);
   float4 noise2 = tex2D(fire_distortion, tc3);

   // Weighted sum of signed noise
   float4 noiseSum = bx2(noise0.r) * distortion_amount0 + bx2(noise1.r) * distortion_amount1 + bx2(noise2.r) * distortion_amount2;

   // Perturb base coordinates in direction of noiseSum as function of height (y)
   float4 perturbedBaseCoords = tc0 + noiseSum * (tc0.y * height_attenuation.x + height_attenuation.y);

   // Sample base and opacity maps with perturbed coordinates
   float4 base = tex2D(fire_base, perturbedBaseCoords);
   float4 opacity = tex2D(fire_opacity, perturbedBaseCoords);

   return base * opacity;
}


//--------------------------------------------------------------//
// Fire_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string Fire_Effects_Fire_ASM_Single_Pass_ScreenAlignedQuad : ModelData = "D:\\RenderMonkey\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader Fire_Effects_Fire_ASM_Single_Pass_Vertex_Shader_ = asm
{
   vs.1.1
   
   dcl_position   v0
   dcl_texcoord   v1
   
   mov  r0, v0        // align the quad with the screen
   mov  r0.w, c0.z
   mov  r0.z, c0.x
   mov  oPos, r0
   
   mov  oT0, v1
   
   mov  r0.xy, c1
   
   mov  r1, v1                  // base tex0 coord plus scaled time
   mad  r1.y, c5.x, r0.y, r1.y
   
   mov  r2, v1                  // base tex1 coord plus scaled time
   mad  r2.y, c5.y, r0.y, r2.y
   
   mov  r3, v1                  // base tex2 coord plus scaled time
   mad  r3.y, c5.z, r0.y, r3.y
   
   mov  oT1, r1
   mov  oT2, r2
   mov  oT3, r3
   
   
   
   
   
   
   
   
   
   
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader Fire_Effects_Fire_ASM_Single_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   texcrd r0.rgb, t0 ; base coord
   texld  r2, t1     ; noise0
   texld  r3, t2     ; noise1
   texld  r4, t3     ; noise2
   
      mul  r2.rgb, c1, r2_bx2     ; noise0 * dis0
      mad  r2.rgb, c2, r3_bx2, r2 ; noise1 * dis1 + noise0 * dis0
      mad  r2.rgb, c3, r4_bx2, r2 ; noise2 * dis2 + noise1 * dis1 + noise0 * dis0
      +mad r0.a, r0.g, c4.x, c4.y ; scale and bias y coord of base map
      mad  r2.rgb, r2, r0.a, r0   ; mult distortion by scaled biased y coord
   
   phase
   
   texld r0, r2 ; index base map with distorted tex coord
   texld r1, r2 ; index opacity map with distorted tex coord
   
   mul r0, r1, r0 ; scale base color by opacity
   
   
   
   
   
   
   
   
   
   
   
   
   
   

};

//--------------------------------------------------------------//
// Technique Section for Fire Effects
//--------------------------------------------------------------//
technique FireBall
{
   pass Single_Pass
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 Fire_Effects_FireBall_Single_Pass_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Fire_Effects_FireBall_Single_Pass_Pixel_Shader_main();
   }

}

technique Explosion
{
   pass Single_Pass
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 Fire_Effects_Explosion_Single_Pass_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Fire_Effects_Explosion_Single_Pass_Pixel_Shader_main();
   }

}

technique Lava
{
   pass Single_Pass
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 Fire_Effects_Lava_Single_Pass_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Fire_Effects_Lava_Single_Pass_Pixel_Shader_main();
   }

}

technique Inferno
{
   pass Single_Pass
   {
      CULLMODE = CCW;

      VertexShader = compile vs_1_1 Fire_Effects_Inferno_Single_Pass_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Fire_Effects_Inferno_Single_Pass_Pixel_Shader_main();
   }

}

technique Fire
{
   pass Single_Pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      SHADEMODE = GOURAUD;
      ZWRITEENABLE = TRUE;
      ALPHATESTENABLE = FALSE;
      LASTPIXEL = TRUE;
      SRCBLEND = ONE;
      DESTBLEND = ZERO;
      CULLMODE = NONE;
      ALPHAREF = 0x0;
      ALPHAFUNC = LESS;
      DITHERENABLE = FALSE;
      ALPHABLENDENABLE = FALSE;
      FOGENABLE = FALSE;
      SPECULARENABLE = FALSE;
      FOGCOLOR = 0xFFFFFFFF;
      FOGTABLEMODE = NONE;
      FOGSTART = 0.000000;
      FOGEND = 0.000000;
      FOGDENSITY = 0.000000;
      STENCILENABLE = FALSE;
      STENCILFAIL = KEEP;
      STENCILZFAIL = KEEP;
      STENCILPASS = KEEP;
      STENCILFUNC = ALWAYS;
      STENCILREF = 0x0;
      STENCILMASK = 0xffffffff;
      STENCILWRITEMASK = 0xffffffff;
      TEXTUREFACTOR = 0x0;
      WRAP0 = 0;
      WRAP1 = 0;
      WRAP2 = 0;
      WRAP3 = 0;
      WRAP4 = 0;
      WRAP5 = 0;
      WRAP6 = 0;
      WRAP7 = 0;
      CLIPPING = FALSE;
      LIGHTING = FALSE;
      AMBIENT = 0x11111111;
      FOGVERTEXMODE = NONE;
      COLORVERTEX = TRUE;
      LOCALVIEWER = TRUE;
      NORMALIZENORMALS = FALSE;
      DIFFUSEMATERIALSOURCE = COLOR1;
      SPECULARMATERIALSOURCE = COLOR2;
      AMBIENTMATERIALSOURCE = COLOR2;
      EMISSIVEMATERIALSOURCE = COLOR2;
      VERTEXBLEND = DISABLE;
      CLIPPLANEENABLE = 0;
      POINTSIZE = 0.000000;
      POINTSIZE_MIN = 0.000000;
      POINTSPRITEENABLE = FALSE;
      POINTSCALEENABLE = FALSE;
      POINTSCALE_A = 0.000000;
      POINTSCALE_B = 0.000000;
      POINTSCALE_C = 0.000000;
      MULTISAMPLEANTIALIAS = TRUE;
      MULTISAMPLEMASK = 0xffffffff;
      PATCHEDGESTYLE = DISCRETE;
      POINTSIZE_MAX = 0.000000;
      INDEXEDVERTEXBLENDENABLE = FALSE;
      COLORWRITEENABLE = RED | GREEN | BLUE | ALPHA;
      TWEENFACTOR = 0.000000;
      BLENDOP = ADD;
      POSITIONDEGREE = CUBIC;
      NORMALDEGREE = LINEAR;
      ZFUNC = ALWAYS;

      VertexShader = compile vs_1_1 Fire_Effects_Fire_Single_Pass_Vertex_Shader_main();
      PixelShader = compile ps_2_0 Fire_Effects_Fire_Single_Pass_Pixel_Shader_main();
   }

}

technique Fire_ASM
{
   pass Single_Pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      SHADEMODE = GOURAUD;
      ZWRITEENABLE = TRUE;
      ALPHATESTENABLE = FALSE;
      LASTPIXEL = TRUE;
      SRCBLEND = ONE;
      DESTBLEND = ZERO;
      CULLMODE = NONE;
      ALPHAREF = 0x0;
      ALPHAFUNC = LESS;
      DITHERENABLE = FALSE;
      ALPHABLENDENABLE = FALSE;
      FOGENABLE = FALSE;
      SPECULARENABLE = FALSE;
      FOGCOLOR = 0xFFFFFFFF;
      FOGTABLEMODE = NONE;
      FOGSTART = 0.000000;
      FOGEND = 0.000000;
      FOGDENSITY = 0.000000;
      STENCILENABLE = FALSE;
      STENCILFAIL = KEEP;
      STENCILZFAIL = KEEP;
      STENCILPASS = KEEP;
      STENCILFUNC = ALWAYS;
      STENCILREF = 0x0;
      STENCILMASK = 0xffffffff;
      STENCILWRITEMASK = 0xffffffff;
      TEXTUREFACTOR = 0x0;
      WRAP0 = 0;
      WRAP1 = 0;
      WRAP2 = 0;
      WRAP3 = 0;
      WRAP4 = 0;
      WRAP5 = 0;
      WRAP6 = 0;
      WRAP7 = 0;
      CLIPPING = FALSE;
      LIGHTING = FALSE;
      AMBIENT = 0x11111111;
      FOGVERTEXMODE = NONE;
      COLORVERTEX = TRUE;
      LOCALVIEWER = TRUE;
      NORMALIZENORMALS = FALSE;
      DIFFUSEMATERIALSOURCE = COLOR1;
      SPECULARMATERIALSOURCE = COLOR2;
      AMBIENTMATERIALSOURCE = COLOR2;
      EMISSIVEMATERIALSOURCE = COLOR2;
      VERTEXBLEND = DISABLE;
      CLIPPLANEENABLE = 0;
      POINTSIZE = 0.000000;
      POINTSIZE_MIN = 0.000000;
      POINTSPRITEENABLE = FALSE;
      POINTSCALEENABLE = FALSE;
      POINTSCALE_A = 0.000000;
      POINTSCALE_B = 0.000000;
      POINTSCALE_C = 0.000000;
      MULTISAMPLEANTIALIAS = TRUE;
      MULTISAMPLEMASK = 0xffffffff;
      PATCHEDGESTYLE = DISCRETE;
      POINTSIZE_MAX = 0.000000;
      INDEXEDVERTEXBLENDENABLE = FALSE;
      COLORWRITEENABLE = RED | GREEN | BLUE | ALPHA;
      TWEENFACTOR = 0.000000;
      BLENDOP = ADD;
      POSITIONDEGREE = CUBIC;
      NORMALDEGREE = LINEAR;
      ZFUNC = ALWAYS;

      VertexShader = (Fire_Effects_Fire_ASM_Single_Pass_Vertex_Shader_);
      PixelShader = (Fire_Effects_Fire_ASM_Single_Pass_Pixel_Shader_);
   }

}

