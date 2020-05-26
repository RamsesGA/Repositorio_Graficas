//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer cbNeverChanges : register( b0 )
{
    matrix View;
};

cbuffer cbChangeOnResize : register( b1 )
{
    matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
    matrix World;
    //Constant Buffers
    float4 lightDir;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 vsPos : POSITION;
    float4 vsNorm : NORMAL0;
    float2 vsTex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 psPos : SV_POSITION;
    float4 psNorm : NORMAL0;
    float2 psTex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.psPos = mul( input.vsPos, World );
    output.psPos = mul( output.psPos, View );
    output.psPos = mul( output.psPos, Projection );
    output.psTex = input.vsTex;
    
    //Estamos multiplicando las normales por el espacio del mundo
    output.psNorm = mul(input.vsNorm, World);
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
   //Luz
   float4 pslight = normalize(-lightDir);

   //Encontrar la inscidencia, producto punto
   float psNormDirLight = dot(input.psNorm, pslight);

   //Multiplicación para poder obtener el color y la luz
   return txDiffuse.Sample( samLinear, input.psTex ) * psNormDirLight;
}