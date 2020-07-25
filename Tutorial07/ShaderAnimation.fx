//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

#define MAX_BONES 100

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer CBNeverChanges : register( b0 )
{
    matrix mView;
};

cbuffer CBChangeOnResize : register( b1 )
{
    matrix mProjection;
};

cbuffer CBChangesEveryFrame : register( b2 )
{
    matrix mWorld;
    float4 vMeshColor;
    float4 vViewPos;
};

cbuffer Lights : register(b3)
{
    float4 mLightDir;
    float3 lightPointPos;
    float4 lightPointAtt;
};

cbuffer ConstBufferBones : register(b4)
{
    matrix bones[MAX_BONES];
}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 msPos        : POSITION0;
    float2 texcoord     : TEXCOORD0;
    int4 boneID         : BLENDINDICES0;
    float4 weights      : BLENDWEIGHT0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : COLOR0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    matrix boneTransform =  mul(input.weights.x, bones[input.boneID.x]);
           boneTransform += mul(input.weights.y, bones[input.boneID.y]);
           boneTransform += mul(input.weights.z, bones[input.boneID.z]);
           boneTransform += mul(input.weights.w, bones[input.boneID.w]);

    output.Pos = mul(boneTransform, float4(input.msPos.xyz, 1.0));

    //output.Pos = mul(float4(input.msPos.xyz, 1.0), mWorld);

    output.Pos = mul( output.Pos, mWorld );
    output.Pos = mul( output.Pos, mView );
    output.Pos = mul( output.Pos, mProjection );
    output.Tex = input.texcoord;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    return txDiffuse.Sample( samLinear, input.Tex ) * vMeshColor;
}
