//--------------------------------------------------------------------------------------
// File: ShadowVolume.fx
//
// The effect file for the MultiAnimation2 sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


#include "skin.vsh"

float4x4 worldMatrix : WORLD;
float4x4 viewProjectionMatrix : VIEWPROJECTION;

texture texScene;


//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler g_samScene =
sampler_state
{
	Texture = <texScene>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Point;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4  Pos             : POSITION;
	float3  BlendWeights    : BLENDWEIGHT;
	float4  BlendIndices    : BLENDINDICES;
	float3  Normal          : NORMAL;
	float3  Tex0            : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4  Pos     : POSITION;
	float4  Diffuse : COLOR0;
	float2  Tex0    : TEXCOORD0;
};


VS_OUTPUT VertScene( float4 Pos : POSITION,
					 float3 Normal : NORMAL,
					 float2 Tex0 : TEXCOORD0 )
{
	VS_OUTPUT o;
	
	o.Pos = mul( Pos, worldMatrix );
	o.Pos = mul( o.Pos, viewProjectionMatrix );
	o.Tex0 = Tex0;
	float3 N = normalize( mul( Normal, (float3x3)worldMatrix ) );

	// Always fully lit the floor
	o.Diffuse = 1.0f;
	
	return o;
}

float4 PixScene( float4 Diffuse : COLOR0,
				 float2 Tex0 : TEXCOORD0 ) : COLOR0
{
	//return tex2D( g_samScene, Tex0 ) * Diffuse;
	return float4(1.0, 0.0, 0, 1);
}


VS_OUTPUT VertSkinning( VS_INPUT i, uniform int iNumBones )
{
	VS_OUTPUT   o;
	
	// skin VB inputs
	VS_SKIN_INPUT vsi = { i.Pos, i.BlendWeights, i.BlendIndices, i.Normal };
	VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

	// transform position from world space into view and then projection space
	o.Pos = mul( float4( vso.vPos.xyz, 1.0f ), viewProjectionMatrix );
	
	// copy the input texture coordinate through
	o.Tex0  = i.Tex0.xy;

	o.Diffuse = float4( 1.f, 1.f, 1.f, 1.f );

	return o;
}


int CurNumBones = 2;

VertexShader vsArray20[ 4 ] = { compile vs_2_0 VertSkinning( 1 ),
								compile vs_2_0 VertSkinning( 2 ),
								compile vs_2_0 VertSkinning( 3 ),
								compile vs_2_0 VertSkinning( 4 ) };


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------


technique RenderScene
{
	pass p0
	{
		VertexShader = compile vs_2_0 VertScene();
		PixelShader = compile ps_2_0 PixScene();
	}
}


technique Skinning20
{
	pass p0
	{
		VertexShader = ( vsArray20[ CurNumBones ] );
		PixelShader = compile ps_2_0 PixScene();
	}
}
