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
// Fog
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
float4x4 matWorld;
float4x4 matWorldView;
float4x4 matWorldViewProj;
float4 fFog;
float4 Camera;
float4 colorFog;

struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Diffuse : COLOR0;
	float2 Tex0		:TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float3 Diffuse : COLOR;
	float3 FogVal : COLOR1;
	float2 Tex0 : TEXCOORD0;
	float  Fog : FOG;
};

struct PS_OUTPUT
{
	float4 Color:COLOR;
};

texture baseTexture
<
	string ResourceName = ".\\";
>;
uniform sampler2D fogbaseSampler = sampler_state
{
	Texture = (baseTexture);
};
//uniform sampler2D baseTexture;

void exp2_main_vs(in const VS_INPUT Input, out VS_OUTPUT Out)
{
	float4 clpPos, worldPos;

	Out = (VS_OUTPUT)0;

	clpPos = mul(Input.Position, matWorldViewProj);
	Out.Position = clpPos;

	Out.Tex0.xy = Input.Tex0.xy;
	Out.Diffuse = Input.Diffuse;

	float fDensity = fFog.x;
	float fFogEnd = fFog.y;

	worldPos = mul(Input.Position, matWorld);

	float fDistance = distance(worldPos, Camera);

	float fDist = fDistance / fFogEnd * 4;

	float f = exp(-(fDist*fDensity)*(fDist*fDensity));

	Out.FogVal.x = f;

}

void layer_main_vs(in const VS_INPUT Input, out VS_OUTPUT Out)
{
	float4 clpPos, camPos, worldPos;
	float fDistance;

	Out = (VS_OUTPUT)0;

	clpPos = mul(Input.Position, matWorldViewProj);
	Out.Position = clpPos;

	Out.Tex0.xy = Input.Tex0.xy;
	Out.Diffuse = Input.Diffuse;

	float fFogTop = fFog.x;
	float fFogEnd = fFog.y;
	float fFogRange = fFog.z;

	worldPos = mul(Input.Position, matWorld);

	fDistance = distance(worldPos, Camera);

	float4 vCameraProj, vWorldProj;
	vCameraProj = Camera;
	vCameraProj.y = 0;
	vWorldProj = worldPos;
	vWorldProj.y = 0;

	float fDeltaD = distance(vCameraProj, vWorldProj) / fFogEnd*2.0f;

	float fDeltaY, fDensityIntergral;
	if (Camera.y > fFogTop)
	{
		if (worldPos.y < fFogTop)
		{
			fDeltaY = (fFogTop - worldPos.y) / fFogRange * 2.0f;
			fDensityIntergral = (fDeltaY*fDeltaY*0.5f);
		}
		else
		{
			fDeltaY = 0.0f;
			fDensityIntergral = 0.0f;
		}
	}
	else
	{
		if (worldPos.y < fFogTop)
		{
			float fDeltaA = (fFogTop - Camera.y) / fFogRange * 2.0f;
			float fDeltaB = (fFogTop - worldPos.y) / fFogRange * 2.0f;
			fDeltaY = abs(fDeltaA - fDeltaB);
			fDensityIntergral = abs((fDeltaA*fDeltaA*0.5f) - (fDeltaB*fDeltaB*0.5f));
		}
		else
		{
			fDeltaY = abs(fFogTop - Camera.y) / fFogRange * 2.0f;
			fDensityIntergral = abs(fDeltaY*fDeltaY*0.5f);
		}
	}
	float fDensity;
	if(fDeltaY!=0.0f)
	{
		fDensity = (sqrt(1.0f + ((fDeltaD / fDeltaY)*(fDeltaD / fDeltaY))))*fDensityIntergral;
	}
	else
	{
		fDensity = 0.0f;
	}

	float f = exp(-fDensity);

	Out.FogVal.x = f;
}

void main_fs(in const VS_OUTPUT Input, out PS_OUTPUT Out)
{
	Out = (PS_OUTPUT)0;
	float4 colorBase = tex2D(fogbaseSampler, Input.Tex0);

	float f = Input.FogVal.x;
	Out.Color = lerp(colorFog, colorBase, f);
}

int Color;

technique FOG
{
	pass P0
	{
		pixelshader = null;
		vertexshader = null;
		fvf = XYZ | Normal;
		LIGHTING = TRUE;
		NORMALIZENORMALS = TRUE;
		SPECULARENABLE = FALSE;

		FOGENABLE = TRUE;
		FOGVERTEXMODE = LINEAR;
		FOGSTART = float(fFog.x);
		FOGEND = float(fFog.y);
		FOGCOLOR = int(Color);
	}

	pass P1
	{
		vertexshader = compile vs_1_1 exp2_main_vs();
		pixelshader = compile ps_2_0 main_fs();
	}

	pass P2
	{
		vertexshader = compile vs_1_1 layer_main_vs();
		pixelshader = compile ps_2_0 main_fs();
	}

}
