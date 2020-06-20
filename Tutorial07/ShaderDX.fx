//--------------------------------------------------------------------------------------
// File: LightShader.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
//#define BLINN
//#define PIXEL_LIGHT
#define NORMAL_MAP_LIGHT

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

Texture2D txDiffuse     : register(t0);
Texture2D Model_n       : register(t1);
SamplerState samLinear  : register(s0);

cbuffer CBNeverChanges  : register(b0)
{
    matrix mView;
};

cbuffer CBChangeOnResize : register(b1)
{
    matrix mProjection;
};

cbuffer CBChangesEveryFrame : register(b2)
{
    matrix mWorld;
    float4 vMeshColor;
    float4 vViewPos;
};

cbuffer Lights: register(b3)
{
    float4 mLightDir;
    float3 lightPointPos;
    float4 lightPointAtt;
};

//--------------------------------------------------------------------------------------
// Output layout
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 color : COLOR0;
};

//--------------------------------------------------------------------------------------
//Input layout
//--------------------------------------------------------------------------------------
struct VS_INPUT {

    float3 msPos        : POSITION0;
    float3 msNormal     : NORMAL0;
    float3 msBinormal   : BINORMAL0;
    float3 msTangent    : TANGENT0;
    float2 texcoord     : TEXCOORD0;
};

//Output layout
struct VS_OUTPUT
{
    float4 wsPos            : SV_POSITION;
    float2 texcoord         : TEXCOORD0;

#ifdef VERTEX_LIGHT
    float3 color            : TEXCOORD1;
#endif

#ifdef PIXEL_LIGHT 
    float3 wsNormal         : NORMAL0;
#endif   

#ifdef NORMAL_MAP_LIGHT

    float3 wsNormal         : NORMAL0;
    float3 wsBinormal       : BINORMAL0;
    float3 wsTangent        : TANGENT0;
#endif
};

//Main function
VS_OUTPUT vs_main(VS_INPUT Input)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;

    //Position in World Space
    float4 wsPos = mul(float4(Input.msPos.xyz, 1.0), mWorld);

    //Position in View Space
    float4 vsPos = mul(wsPos, mView);

    //Position in Projection Space
    float4 psPos = mul(vsPos, mProjection);
    wsPos = psPos;
    Output.wsPos = psPos;

    //Normal in World Space
    float3 wsNormal = normalize(mul(float4(Input.msNormal.xyz, 0.0), mWorld).xyz);

#ifdef NORMAL_MAP_LIGHT
    float3 wsBinormal = normalize(mul(float4(Input.msBinormal.xyz, 0.0), mWorld).xyz);
    float3 wsTangent = normalize(mul(float4(Input.msTangent.xyz, 0.0), mWorld).xyz);
#endif   

    //Vertex Light
#ifdef VERTEX_LIGHT

   //ViewDirection
    //float3 vPos = float3(mView[0][3], mView[1][3], mView[2][3]);
    float3 wsViewDir = normalize(wsPos.xyz - vViewPos.xyz);
    float3 diffuse = float3(0, 0, 0);
    float3 specular = float3(0, 0, 0);
    float3 diffuse2 = float3(1, 0, 1);
    float3 specular2 = float3(1, 1, 1);

    //dir light
    {
        //Light
        float3 light = normalize(-mLightDir);

        //Dot product
        float Ndl = clamp(dot(wsNormal, light), 0, 1);

        //Diffuse Ligth
        diffuse += diffuse2 * Ndl;

#ifdef BLINN
        //Reflection Vector
        float3 wsReflection = normalize(reflect(-light, wsNormal));

        //Specular light
        float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1), 16) * Ndl;
        specular += specular2 * specPow;
#endif
#ifdef BLINN_PHONG
        //Half
        float3 wsHalf = normalize(light + wsViewDir);

        //Specular light
        float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1), 16) * Ndl;
        specular += specular2 * specPow;
#endif
    }

    //point light
    {
        //Light
        float3 light = normalize(wsPos - lightPointPos.xyz);
        float attenuation = clamp(length(light) / 0.1, 0, 1);

        attenuation = length(light) / lightPointAtt.x;
        light = normalize(light);

        //Dot product
        float Ndl = 1.0 - clamp(dot(wsNormal, light), 0, 1) * attenuation;

        //Diffuse Ligth
        diffuse += diffuse2 * Ndl;

#ifdef BLINN
        //Reflection Vector
        float3 wsReflection = normalize(reflect(-light, wsNormal));

        //Specular light
        float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1), 16) * Ndl;
        specular += specular2 * specPow;
#endif

#ifdef BLINN_PHONG
        //Half
        float3 wsHalf = normalize(light + wsViewDir);

        //Specular light
        float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1), 16) * Ndl;
        specular += specular2 * specPow;
#endif
    }
    //Outut
    Output.color = diffuse.xyz + specular.xyz;

#endif 

#ifdef PIXEL_LIGHT
    //out
    Output.wsNormal = wsNormal;
#endif

    //pixel light
#ifdef NORMAL_MAP_LIGHT
    Output.wsNormal = wsNormal.xyz;
    Output.wsBinormal = wsBinormal.xyz;
    Output.wsTangent = wsTangent.xyz;
    
#endif 
    Output.texcoord = Input.texcoord;
    Output.wsPos = psPos;
    return Output;
}


PS_OUTPUT ps_main(VS_OUTPUT Input)
{
    PS_OUTPUT Output = (PS_OUTPUT)0;

    //VertexLight
#ifdef VERTEX_LIGHT
    Output.color = float4(Input.color.xyz, 1);
#endif
    float3 wsNormal;
#ifdef NORMAL_MAP_LIGHT
    float3x3 TBN = float3x3(Input.wsTangent.xyz, Input.wsBinormal.xyz, Input.wsNormal.xyz);
    wsNormal = Model_n.Sample(samLinear, Input.texcoord.xy);
    wsNormal.xyz = (wsNormal.xyz * 2.0) - 1.0;
    wsNormal.xyz = normalize(mul(wsNormal.xyz, TBN).xyz);
#endif 

    //pixel
#ifdef PIXEL_LIGHT
    wsNormal = normalize(Input.wsNormal.xyz);
#endif

#if  defined(PIXEL_LIGHT) ||  defined(NORMAL_MAP_LIGHT)
    //ViewDirection
    float3 vPos = { mView[0][3], mView[1][3], mView[2][3] };
    float3 wsViewDir = normalize(Input.wsPos.xyz - vViewPos.xyz);
    
    float3 diffuse = float3(0, 0, 0);
    float3 specular = float3(0, 0, 0);
    float3 diffuse2 = float3(1, 0, 1);
    float3 specular2 = float3(1, 1, 1);

    //dir light
//    {
//        //Light
//        float3 light = normalize(-mLightDir.xyz);
//
//        //Dot product
//        float Ndl = clamp(dot(wsNormal, light), 0, 1);
//
//        //Diffuse Ligth
//        diffuse += diffuse2 * Ndl;
//
//#ifdef BLINN
//        //Reflection Vector
//        float3 wsReflection = normalize(reflect(-light, wsNormal));
//
//        //Specular light
//        float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1), 16) * Ndl;
//        specular += specular2 * specPow;
//#endif
//
//#ifdef BLINN_PHONG
//
//        //Half
//        float3 wsHalf = normalize(light + wsViewDir.xyz);
//
//        //Specular light
//        float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1), 16) * Ndl;
//        specular += specular2 * specPow;
//#endif
//    }

    //point light
    {
        //Light
        float3 light = normalize(Input.wsPos.xyz - lightPointPos.xyz);
        //light = normalize(float3 (0, 100, 0) - Input.wsPos.xyz);
        float attenuation = clamp(length(light) / 0.1, 0, 1);

        attenuation = length(light) / lightPointAtt.x;
        //attenuation = length(light) / 0.2;
        light = normalize(light);

        //Dot product
        float Ndl = 1.0 - clamp(dot(wsNormal, light), 0, 1) * attenuation;

        //Diffuse Ligth
        diffuse += diffuse2 * Ndl;

#ifdef BLINN
        //Reflection Vector
        float3 wsReflection = normalize(reflect(-light, wsNormal));

        //Specular light
        float specPow = pow(clamp(dot(wsViewDir.xyz, wsReflection.xyz), 0, 1), 16) * Ndl;
        specular += specular2 * specPow;
#endif

#ifdef BLINN_PHONG
        //Half
        float3 wsHalf = normalize(light + wsViewDir);

        //Specular light
        float specPow = pow(clamp(dot(wsNormal.xyz, wsHalf.xyz), 0, 1), 16) * Ndl;
        specular += specular2 * specPow;
#endif
    }

    //Output.color = float4(diffuse.xyz + specular.xyz, 1);

    Output.color = txDiffuse.Sample(samLinear, Input.texcoord) * float4(diffuse.xyz + specular.xyz, 1);
#endif
    return Output;
}