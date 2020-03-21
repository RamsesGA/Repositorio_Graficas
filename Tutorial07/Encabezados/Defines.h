#pragma once
#define D3D11
#define OPENGL

#include <windows.h>
#include "mathfu/hlsl_mappings.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//DIRECTX
#ifdef D3D11
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif // D3D11

//OPENGL
#include <glew.h>
#include <GLFWInclude/x32/glfw3.h>



struct SimpleVertex
{
    //XMFLOAT3 Pos;
    mathfu::float3 Pos;
    //XMFLOAT2 Tex;
    mathfu::float2 Tex;
};

struct CBChangesEveryFrame
{
    //XMMATRIX mWorld;
    mathfu::float4x4 mWorld;
    //XMFLOAT4 vMeshColor;
    mathfu::float4 vMeshColor;
};

/***********
	Device
***********/
struct TEX2D_DSV{

    UINT My_MipSlice;
};

enum DSV_DIMENSION{

    DSV_DIMENSION_UNKNOWN = 0,
    DSV_DIMENSION_TEXTURE1D = 1,
    DSV_DIMENSION_TEXTURE1DARRAY = 2,
    DSV_DIMENSION_TEXTURE2D = 3,
    DSV_DIMENSION_TEXTURE2DARRAY = 4,
    DSV_DIMENSION_TEXTURE2DMS = 5,
    DSV_DIMENSION_TEXTURE2DMSARRAY = 6
};

struct SUBRESOURCE_DATA {

    const void* My_pSysMem;
    unsigned int My_SysMemPitch;
    unsigned int My_SysMemSlicePitch;
};

//D3D_DRIVER_TYPE		g_driverType
enum DRIVER_TYPE{

	DRIVER_TYPE_UNKNOWN = 0,
	DRIVER_TYPE_HARDWARE = (DRIVER_TYPE_UNKNOWN + 1),
	DRIVER_TYPE_REFERENCE = (DRIVER_TYPE_HARDWARE + 1),
	DRIVER_TYPE_NULL = (DRIVER_TYPE_REFERENCE + 1),
	DRIVER_TYPE_SOFTWARE = (DRIVER_TYPE_NULL + 1),
	DRIVER_TYPE_WARP = (DRIVER_TYPE_SOFTWARE + 1)
};

//D3D_FEATURE_LEVEL	g_featureLevel
enum FEATURE_LEVEL{

	FEATURE_LEVEL_9_1 = 0x9100,
	FEATURE_LEVEL_9_2 = 0x9200,
	FEATURE_LEVEL_9_3 = 0x9300,
	FEATURE_LEVEL_10_0 = 0xa000,
	FEATURE_LEVEL_10_1 = 0xa100,
	FEATURE_LEVEL_11_0 = 0xb000
};

/***********
	Viewport
***********/

//D3D11_VIEWPORT vp;
struct VIEWPORT{

	float My_TopLeftX;
	float My_TopLeftY;
	float My_Width;
	float My_Height;
	float My_MinDepth;
	float My_MaxDepth;
};

/***********
	Texture2D
***********/

//DXGI_FORMAT Format
enum FORMAT{

    FORMAT_UNKNOWN = 0,
    FORMAT_R32G32B32A32_TYPELESS = 1,
    FORMAT_R32G32B32A32_FLOAT = 2,
    FORMAT_R32G32B32A32_UINT = 3,
    FORMAT_R32G32B32A32_SINT = 4,
    FORMAT_R32G32B32_TYPELESS = 5,
    FORMAT_R32G32B32_FLOAT = 6,
    FORMAT_R32G32B32_UINT = 7,
    FORMAT_R32G32B32_SINT = 8,
    FORMAT_R16G16B16A16_TYPELESS = 9,
    FORMAT_R16G16B16A16_FLOAT = 10,
    FORMAT_R16G16B16A16_UNORM = 11,
    FORMAT_R16G16B16A16_UINT = 12,
    FORMAT_R16G16B16A16_SNORM = 13,
    FORMAT_R16G16B16A16_SINT = 14,
    FORMAT_R32G32_TYPELESS = 15,
    FORMAT_R32G32_FLOAT = 16,
    FORMAT_R32G32_UINT = 17,
    FORMAT_R32G32_SINT = 18,
    FORMAT_R32G8X24_TYPELESS = 19,
    FORMAT_D32_FLOAT_S8X24_UINT = 20,
    FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
    FORMAT_X32_TYPELESS_G8X24_UINT = 22,
    FORMAT_R10G10B10A2_TYPELESS = 23,
    FORMAT_R10G10B10A2_UNORM = 24,
    FORMAT_R10G10B10A2_UINT = 25,
    FORMAT_R11G11B10_FLOAT = 26,
    FORMAT_R8G8B8A8_TYPELESS = 27,
    FORMAT_R8G8B8A8_UNORM = 28,
    FORMAT_R8G8B8A8_UNORM_SRGB = 29,
    FORMAT_R8G8B8A8_UINT = 30,
    FORMAT_R8G8B8A8_SNORM = 31,
    FORMAT_R8G8B8A8_SINT = 32,
    FORMAT_R16G16_TYPELESS = 33,
    FORMAT_R16G16_FLOAT = 34,
    FORMAT_R16G16_UNORM = 35,
    FORMAT_R16G16_UINT = 36,
    FORMAT_R16G16_SNORM = 37,
    FORMAT_R16G16_SINT = 38,
    FORMAT_R32_TYPELESS = 39,
    FORMAT_D32_FLOAT = 40,
    FORMAT_R32_FLOAT = 41,
    FORMAT_R32_UINT = 42,
    FORMAT_R32_SINT = 43,
    FORMAT_R24G8_TYPELESS = 44,
    FORMAT_D24_UNORM_S8_UINT = 45,
    FORMAT_R24_UNORM_X8_TYPELESS = 46,
    FORMAT_X24_TYPELESS_G8_UINT = 47,
    FORMAT_R8G8_TYPELESS = 48,
    FORMAT_R8G8_UNORM = 49,
    FORMAT_R8G8_UINT = 50,
    FORMAT_R8G8_SNORM = 51,
    FORMAT_R8G8_SINT = 52,
    FORMAT_R16_TYPELESS = 53,
    FORMAT_R16_FLOAT = 54,
    FORMAT_D16_UNORM = 55,
    FORMAT_R16_UNORM = 56,
    FORMAT_R16_UINT = 57,
    FORMAT_R16_SNORM = 58,
    FORMAT_R16_SINT = 59,
    FORMAT_R8_TYPELESS = 60,
    FORMAT_R8_UNORM = 61,
    FORMAT_R8_UINT = 62,
    FORMAT_R8_SNORM = 63,
    FORMAT_R8_SINT = 64,
    FORMAT_A8_UNORM = 65,
    FORMAT_R1_UNORM = 66,
    FORMAT_R9G9B9E5_SHAREDEXP = 67,
    FORMAT_R8G8_B8G8_UNORM = 68,
    FORMAT_G8R8_G8B8_UNORM = 69,
    FORMAT_BC1_TYPELESS = 70,
    FORMAT_BC1_UNORM = 71,
    FORMAT_BC1_UNORM_SRGB = 72,
    FORMAT_BC2_TYPELESS = 73,
    FORMAT_BC2_UNORM = 74,
    FORMAT_BC2_UNORM_SRGB = 75,
    FORMAT_BC3_TYPELESS = 76,
    FORMAT_BC3_UNORM = 77,
    FORMAT_BC3_UNORM_SRGB = 78,
    FORMAT_BC4_TYPELESS = 79,
    FORMAT_BC4_UNORM = 80,
    FORMAT_BC4_SNORM = 81,
    FORMAT_BC5_TYPELESS = 82,
    FORMAT_BC5_UNORM = 83,
    FORMAT_BC5_SNORM = 84,
    FORMAT_B5G6R5_UNORM = 85,
    FORMAT_B5G5R5A1_UNORM = 86,
    FORMAT_B8G8R8A8_UNORM = 87,
    FORMAT_B8G8R8X8_UNORM = 88,
    FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
    FORMAT_B8G8R8A8_TYPELESS = 90,
    FORMAT_B8G8R8A8_UNORM_SRGB = 91,
    FORMAT_B8G8R8X8_TYPELESS = 92,
    FORMAT_B8G8R8X8_UNORM_SRGB = 93,
    FORMAT_BC6H_TYPELESS = 94,
    FORMAT_BC6H_UF16 = 95,
    FORMAT_BC6H_SF16 = 96,
    FORMAT_BC7_TYPELESS = 97,
    FORMAT_BC7_UNORM = 98,
    FORMAT_BC7_UNORM_SRGB = 99,
    FORMAT_FORCE_UINT = 0xffffffff
};

//DXGI_SAMPLE_DESC SampleDesc
struct SAMPLE_DESC{

    unsigned int My_Count;
    unsigned int My_Quality;
};

//D3D11_USAGE Usage
enum USAGE{

    USAGE_DEFAULT = 0,
    USAGE_IMMUTABLE = 1,
    USAGE_DYNAMIC = 2,
    USAGE_STAGING = 3
};

/***********
    Swap Chain
***********/

struct RATIONAL {

    unsigned int My_Numerator;
    unsigned int My_Denominator;
};

enum MODE_SCANLINE_ORDER {

    MODE_SCANLINE_ORDER_UNSPECIFIED = 0,
    MODE_SCANLINE_ORDER_PROGRESSIVE = 1,
    MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
    MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
};

enum MODE_SCALING {

    MODE_SCALING_UNSPECIFIED = 0,
    MODE_SCALING_CENTERED = 1,
    MODE_SCALING_STRETCHED = 2
};

//DXGI_MODE_DESC		BufferDesc
struct MODE_DESC {

    unsigned int        My_Width;
    unsigned int        My_Height;
    RATIONAL            My_RefreshRate;
    FORMAT              My_Format;
    MODE_SCANLINE_ORDER My_ScanlineOrdering;
    MODE_SCALING        My_Scaling;
};

//DXGI_SWAP_EFFECT	SwapEffect
enum SWAP_EFFECT {

    SWAP_EFFECT_DISCARD = 0,
    SWAP_EFFECT_SEQUENTIAL = 1
};

//DXGI_SWAP_CHAIN_DESC sd
struct SWAP_CHAIN_DESC {

    MODE_DESC       My_BufferDesc;
    SAMPLE_DESC     My_SampleDesc;
    USAGE           My_BufferUsage;
    unsigned int    My_BufferCount;
    HWND            My_OutputWindow;
    bool            My_Windowed;
    SWAP_EFFECT     My_SwapEffect;
    unsigned int    My_Flags;
};

/***********
    Sample State
***********/

//D3D11_FILTER Filter
enum FILTER {

    FILTER_MIN_MAG_MIP_POINT = 0,
    FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
    FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
    FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
    FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
    FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
    FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
    FILTER_MIN_MAG_MIP_LINEAR = 0x15,
    FILTER_ANISOTROPIC = 0x55,
    FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
    FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
    FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
    FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
    FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
    FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
    FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
    FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
    FILTER_COMPARISON_ANISOTROPIC = 0xd5
};

//D3D11_TEXTURE_ADDRESS_MODE AddressU, AddressV, AddressW
enum TEXTURE_ADDRESS_MODE {

    TEXTURE_ADDRESS_WRAP = 1,
    TEXTURE_ADDRESS_MIRROR = 2,
    TEXTURE_ADDRESS_CLAMP = 3,
    TEXTURE_ADDRESS_BORDER = 4,
    TEXTURE_ADDRESS_MIRROR_ONCE = 5
};

//D3D11_COMPARISON_FUNC ComparisonFunc
enum COMPARISON_FUNC {

    COMPARISON_NEVER = 1,
    COMPARISON_LESS = 2,
    COMPARISON_EQUAL = 3,
    COMPARISON_LESS_EQUAL = 4,
    COMPARISON_GREATER = 5,
    COMPARISON_NOT_EQUAL = 6,
    COMPARISON_GREATER_EQUAL = 7,
    COMPARISON_ALWAYS = 8
};

//D3D11_SAMPLER_DESC sampDesc
struct SAMPLER_DESC{

    FILTER                  My_Filter;
    TEXTURE_ADDRESS_MODE    My_AddressU;
    TEXTURE_ADDRESS_MODE    My_AddressV;
    TEXTURE_ADDRESS_MODE    My_AddressW;
    float                   My_MipLODBias;
    unsigned int            My_MaxAnisotropy;
    COMPARISON_FUNC         My_ComparisonFunc;
    float                   My_BorderColor[4];
    float                   My_MinLOD;
    float                   My_MaxLOD;
};

//D3D11_RTV_DIMENSION ViewDimension
enum RTV_DIMENSION {

    RTV_DIMENSION_UNKNOWN = 0,
    RTV_DIMENSION_BUFFER = 1,
    RTV_DIMENSION_TEXTURE1D = 2,
    RTV_DIMENSION_TEXTURE1DARRAY = 3,
    RTV_DIMENSION_TEXTURE2D = 4,
    RTV_DIMENSION_TEXTURE2DARRAY = 5,
    RTV_DIMENSION_TEXTURE2DMS = 6,
    RTV_DIMENSION_TEXTURE2DMSARRAY = 7,
    RTV_DIMENSION_TEXTURE3D = 8
};

//D3D11_BUFFER_RTV Buffer
struct BUFFER_RTV{

    union{

        unsigned int My_FirstElement;
        unsigned int My_ElementOffset;
    };
    union{

        unsigned int My_NumElements;
        unsigned int My_ElementWidth;
    };
};

//D3D11_TEX1D_RTV Texture1D
struct TEX1D_RTV{

    unsigned int My_MipSlice;
};

//D3D11_TEX1D_ARRAY_RTV Texture1DArray
struct TEX1D_ARRAY_RTV{

    unsigned int My_MipSlice;
    unsigned int My_FirstArraySlice;
    unsigned int My_ArraySize;
};

//D3D11_TEX2D_RTV Texture2D
struct TEX2D_RTV{

    unsigned int My_MipSlice;
};

//D3D11_TEX2D_ARRAY_RTV Texture2DArray
struct TEX2D_ARRAY_RTV{

    unsigned int My_MipSlice;
    unsigned int My_FirstArraySlice;
    unsigned int My_ArraySize;
};

//D3D11_TEX2DMS_RTV Texture2DMS
struct TEX2DMS_RTV{

    unsigned int My_UnusedField_NothingToDefine;
};

//D3D11_TEX2DMS_ARRAY_RTV Texture2DMSArray
struct TEX2DMS_ARRAY_RTV{

    unsigned int My_FirstArraySlice;
    unsigned int My_ArraySize;
};

//D3D11_TEX3D_RTV Texture3D
struct TEX3D_RTV{

    unsigned int My_MipSlice;
    unsigned int My_FirstWSlice;
    unsigned int My_WSize;
};

/***********
    Depth Stencil
***********/

//D3D11_TEXTURE2D_DESC descDepth
struct TEXTURE2D_DESC {

    unsigned int    My_Width;
    unsigned int    My_Height;
    unsigned int    My_MipLevels;
    unsigned int    My_ArraySize;
    FORMAT          My_Format;
    SAMPLE_DESC     My_SampleDesc;
    USAGE           My_Usage;
    unsigned int    My_BindFlags;
    unsigned int    My_CPUAccessFlags;
    unsigned int    My_MiscFlags;
};

enum BIND_FLAG {

    BIND_VERTEX_BUFFER = 0x1L,
    BIND_INDEX_BUFFER = 0x2L,
    BIND_CONSTANT_BUFFER = 0x4L,
    BIND_SHADER_RESOURCE = 0x8L,
    BIND_STREAM_OUTPUT = 0x10L,
    BIND_RENDER_TARGET = 0x20L,
    BIND_DEPTH_STENCIL = 0x40L,
    BIND_UNORDERED_ACCESS = 0x80L
};

struct BUFFER_DESC
{
    UINT ByteWidth;
    USAGE Usage;
    UINT BindFlags;
    UINT CPUAccessFlags;
    UINT MiscFlags;
    UINT StructureByteStride;
};