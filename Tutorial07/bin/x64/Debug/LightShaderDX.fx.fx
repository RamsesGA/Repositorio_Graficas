//--------------------------------------------------------------------------------------
// File: LightShader.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#define BLINN_PHONG
#define NORMAL_MAP_LIGHT

//Defines lock

#ifdef NORMAL_MAP_LIGHT
   #undef VERTEX_LIGHT
   #undef PIXEL_LIGHT
#else
   #ifdef PIXEL_LIGHT
      #undef VERTEX_LIGHT
   #else
        #define VERTEX_LIGHT
     #endif
#endif

#ifdef BLINN_PHONG
   #undef BLINN
#else
   #define BLINN
#endif


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
#ifdef VERTEX_LIGHT
    float4 lightDir;
	float4 vViewPos;
    float3 lightPointPos;
    float lightPointAtt;
#endif

};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Norm : NORMAL0;
	
#ifdef NORMAL_MAP_LIGHT
      float3 msBinormal : BINORMAL0;
      float3 msTangent : TANGENT0;
      float2 Tex : TEXCOORD0; 
#endif
};

struct PS_INPUT
{

#ifdef VERTEX_LIGHT
      float3 color      : TEXCOORD0;
#endif

#ifdef PIXEL_LIGHT 
      float3 Normal   : NORMAL0;
      float3 Pos      : TEXCOORD0;
#endif
  
#ifdef NORMAL_MAP_LIGHT
      float2 Tex : TEXCOORD0;
      float3 Pos      : TEXCOORD1;
      float3x3 TBN : TEXCOORD2;
      
#endif
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{	
	VS_OUTPUT Output;
   
   //Position in World Space
   float4 wsPos = mul( float4(Input.msPos.xyz, 1.0), matWorld );
   
   //Position in View Space
   float4 vsPos = mul( wsPos, matView );
   
   //Position in Projection Space
   float4 psPos = mul( vsPos, matProjection );
   Output.psPos = psPos;
   
   //Normal in World Space
   float3 wsNormal = normalize(mul(float4(Input.msNormal.xyz, 0.0), matWorld).xyz);
   
#ifdef NORMAL_MAP_LIGHT
       float3 wsBinormal = normalize(mul(float4(Input.msBinormal.xyz, 0.0), matWorld).xyz);
       float3 wsTangent = normalize(mul(float4(Input.msTangent.xyz, 0.0), matWorld).xyz);
#endif   
    
    
   //Vertex Light
#ifdef VERTEX_LIGHT
   
      //ViewDirection
      float3 wsViewDir = normalize(vViewPos.xyz - wsPos.xyz/*-vViewDir.xyz*/);
      
      float3 diffuse = float3(0,0,0);
      float3 specular= float3(0,0,0);
      
      //dir light
      {
         //Light
         float3 light = normalize(-lightDir);
         //Dot product
         float Ndl = clamp(dot(wsNormal, light), 0, 1);
         
         //Diffuse Ligth
         diffuse = float4(0.1,0.1,0.1,0.1) * Ndl;
         
#ifdef BLINN
            //Reflection Vector
            float3 wsReflection = normalize(reflect(-light, wsNormal));
        
            //Specular light
            float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;
#endif
#ifdef BLINN_PHONG
            //Half
            float3 wsHalf = normalize(light + wsViewDir);         
         
            //Specular light
            float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;         
#endif
      
      }
	  //point light
       {
          //Light
         float3 light = normalize(lightPointPos.xyz - wsPos);
         
         float attenuation= clamp(length(light)/lightPointAtt, 0, 1);
         
         light = normalize(light);
         //Dot product
         float Ndl = 1.0 - clamp(dot(wsNormal, light), 0, 1)*attenuation;
         
         //Diffuse Ligth
         diffuse = float4(0.1,0.1,0.1,0.1) * Ndl;
         
#ifdef BLINN
            //Reflection Vector
            float3 wsReflection = normalize(reflect(-light, wsNormal));
        
            //Specular light
            float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;
#endif
#ifdef BLINN_PHONG
            //Half
            float3 wsHalf = normalize(light + wsViewDir);         
         
            //Specular light
            float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;         
#endif
      
       }
	   //Outut
      Output.color = diffuse.xyz + specular.xyz;
   
#endif 
      
#ifdef PIXEL_LIGHT
      //out
         Output.wsNormal = wsNormal;
         Output.wsPos = wsPos.xyz;
#endif
   
      //pixel light
#ifdef NORMAL_MAP_LIGHT
         Output.wsPos = wsPos.xyz;
         Output.TBN= float3x3(wsTangent.xyz,wsBinormal.xyz,wsNormal.xyz);
         Output.texcoord=Input.texcoord;
#endif 
   
   
   return Output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
   PS_OUTPUT Output;
   
   //VertexLight
#ifdef VERTEX_LIGHT
      Output.color = float4(Input.color.xyz, 1);
#endif
   
   float3 wsNormal;
   
#ifdef NORMAL_MAP_LIGHT
      wsNormal= tex2D(Model_n, Input.texcoord).xyz;
      wsNormal.xyz=(wsNormal.xyz*2.0)-1.0;
      wsNormal.xyz=normalize(mul(wsNormal.xyz,Input.TBN).xyz);
      
      
#endif 
   
   //pixel
#ifdef PIXEL_LIGHT
      wsNormal=Input.wsNormal;
#endif
   
#if defined(PIXEL_LIGHT) ||  defined(NORMAL_MAP_LIGHT)
     //ViewDirection
      float3 wsViewDir = normalize(vViewPos.xyz - Input.wsPos.xyz/*-vViewDir.xyz*/);
      
      float3 diffuse = float3(0,0,0);
      float3 specular= float3(0,0,0);
	  
	  //dir light
      {
         //Light
         float3 light = normalize(-lightDir);
         //Dot product
         float Ndl = clamp(dot(wsNormal, light), 0, 1);
         
         //Diffuse Ligth
         diffuse = float4(0.1,0.1,0.1,0.1) * Ndl;
         
#ifdef BLINN
            //Reflection Vector
            float3 wsReflection = normalize(reflect(-light, wsNormal));
        
            //Specular light
            float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;
#endif
#ifdef BLINN_PHONG
            //Half
            float3 wsHalf = normalize(light + wsViewDir);         
         
            //Specular light
            float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;         
#endif
      
      }
	  //point light
       {
          //Light
         float3 light = normalize(lightPointPos.xyz - Input.wsPos);
         
         float attenuation= clamp(length(light)/lightPointAtt, 0, 1);
         
         light = normalize(light);
         //Dot product
         float Ndl = 1.0 - clamp(dot(wsNormal, light), 0, 1)*attenuation;
         
         //Diffuse Ligth
         diffuse = float4(0.1,0.1,0.1,0.1) * Ndl;
         
#ifdef BLINN
            //Reflection Vector
            float3 wsReflection = normalize(reflect(-light, wsNormal));
        
            //Specular light
            float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;
#endif
#ifdef BLINN_PHONG
            //Half
            float3 wsHalf = normalize(light + wsViewDir);         
         
            //Specular light
            float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1),16) * Ndl;
            specular += float4(1,1,1,1) * specPow;         
#endif
      
       }
   
   Output.color = float4(diffuse.xyz + specular.xyz, 1);
#endif
 
   return Output;
}