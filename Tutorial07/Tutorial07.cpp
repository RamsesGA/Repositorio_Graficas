//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
//#include <windows.h>
#include "resource.h"
#include "Encabezados/Defines.h"

///
/// Ramses´s libraries
///
#include "Camera.h"
#include "FirstCamera.h"
#include <fstream>
#include <iostream>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <vector>
#include "Encabezados/ClaseDevice.h"
#include "Encabezados/ClaseDeviceContext.h"
#include "Encabezados/ClaseSwapChain.h"
#include "Encabezados/ClaseRenderTargetView.h"
#include "Encabezados/ClaseRenderTarget.h"
#include "Encabezados/ClaseSampleState.h"
#include "Encabezados/ClaseDepthStencil.h"
#include "Encabezados/ClaseViewport.h"
#include "Encabezados/ClaseShader.h"
#include "Encabezados/ClaseVertexBuffer.h"
#include "Encabezados/ClaseTexture2D.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

//API
#include "Encabezados/GraphicApi.h"
#include "Encabezados/SceneManager.h"

//GLFW
#if _WIN32 || _WIN64
#if _WIN64
#include <GLFWInclude/x64/glfw3.h>
#else
#include "Encabezados/ClaseOpenGL.h"
#endif
#endif

//--------------------------------------------------------------------------------------
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

///
/// Structures
///
struct CBNeverChanges
{
    mathfu::float4x4 mView;
};
struct CBChangeOnResize
{
    mathfu::float4x4 mProjection;
};

///
/// Global Variables
///
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;

#ifdef D3D11
///
/// in pixel shader derived from shader class
///
ID3D11PixelShader*          g_pPixelShader  = NULL;

///
/// in material class or shader Resources
///
ID3D11ShaderResourceView*   g_pTextureRV    = NULL;
ID3D11ShaderResourceView*   G_InactiveSRV   = NULL;
#endif

mathfu::float4x4            g_World;
mathfu::float4              g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);

///
/// Ramses´s global variables
///
ClaseDevice                 CDev;
ClaseDeviceContext          CDevCont;
ClaseSwapChain              CSwap;
ClaseRenderTargetView       CRendTarView;
RenderTarget                CRendTar;
ClaseSampleState            CSampleS;
ClaseDepthStencil           CDepthStencilView;
ClaseViewport               CView;
ClaseShader                 CShader;

ClaseBuffer                 FCNeverChange;
ClaseBuffer                 FCOnResize;
ClaseBuffer                 FCChangeEveryFrame;

ClaseVertexBuffer           CVertexBuffer;
ClaseVertexBuffer           CIndexBuffer;

Camera                      FreeCamera;
FirstCamera                 FPSCamera;

ClaseBuffer                 FPCNeverChange;
ClaseBuffer                 FPCOnResize;
ClaseBuffer                 FPCChangeEveryFrame;

ClaseTextura2D              InactiveTexture;
ClaseTextura2D              G_DepthStencil;

ClaseRenderTargetView       InactiveRTV;

Camera*                     MainCamera      = NULL;
Camera*                     SecondCamera    = NULL;
Camera                      CamaraLibre;
Camera                      CamaraPrimeraPersona;

///
/// API
///
SCENEMANAGER                G_SceneManager;
GraphicApi                  G_GraphicApi;

#if defined(D3D11)
ID3D11Device*               ptrDEV;
IDXGISwapChain*             ptrSwap;
ID3D11DeviceContext*        ptrDevCont;
//ID3D11Texture2D* ptrDepht 
#endif

int                         LevelCubes[10][10] = { 0 };
int                         Rows = 0;
int                         Columns = 0;
int                         SwitchCamera = -1;

UINT                        width;
UINT                        height;

///
/// OpenGl variables
///
unsigned int g_ColorShaderID;
ClaseOpenGL g_OpenGlObj;

///
/// Function for resize in DIRECTX
///
void Resize(){

#ifdef D3D11

    if (CDevCont.g_pImmediateContextD3D11 != nullptr) {

        ///
        /// Get new window dimensions
        ///
        RECT rc;
        GetClientRect(g_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        ///
        /// Regenerate world matrix as identity
        ///
        g_World = g_World.Identity();

        ///
        ///Set w and h for camera 
        ///
        MainCamera->SetHeight(height);
        MainCamera->SetWidht(width);

        ///
        /// Update projection matrix with new params
        ///
        MainCamera->UpdateVM();

        ///
        /// Update CB
        ///
        CBChangeOnResize cbChangesOnResize;
        cbChangesOnResize.mProjection = MainCamera->GetProjection();
        ptrDevCont->UpdateSubresource(MainCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize, 0, 0);

        ///
        /// inactive camera
        ///
        SecondCamera->SetHeight(height);
        SecondCamera->SetWidht(width);
        SecondCamera->UpdateVM();

        CBChangeOnResize cbChangesOnResize2;
        cbChangesOnResize2.mProjection = SecondCamera->GetProjection();
        ptrDevCont->UpdateSubresource(SecondCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize2, 0, 0);

        if (ptrSwap) {

            HRESULT h;

            ///
            /// Release inactive camera texture, SRV and RTV
            ///
            InactiveTexture.m_TextureD3D11->Release();
            G_InactiveSRV->Release();
            InactiveRTV.g_pRenderTargetViewD3D11->Release();
	
            ///
            /// Resize inactive camera texture	
            ///
            Texture2Desc TD;
            ZeroMemory(&TD, sizeof(TD));
            TD.Width = width;
            TD.Height = height;
            TD.MipLevels = TD.ArraySize = 1;
            TD.Format = FORMAT_R8G8B8A8_UNORM;
            TD.SampleDesc.My_Count = 1;
            TD.Usage = USAGE_DEFAULT;
            TD.BindFlags = 8 | 32;			// D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
            TD.CPUAccessFlags = 65536;	//D3D11_CPU_ACCESS_WRITE;
            TD.MiscFlags = 0;
            InactiveTexture.Init(TD);

            h = ptrDEV->CreateTexture2D(&InactiveTexture.m_TextDescD3D11, NULL, &InactiveTexture.m_TextureD3D11);

            RenderTargetViewDesc RTVD;
            ZeroMemory(&RTVD, sizeof(RTVD));
            RTVD.Format = TD.Format;
            RTVD.ViewDimension = RTV_DIMENSION_TEXTURE2D;
            RTVD.Texture2D.My_MipSlice = 0;
            InactiveRTV.Init(RTVD);

            h = ptrDEV->CreateRenderTargetView(InactiveTexture.m_TextureD3D11, &InactiveRTV.m_renderTVD3D11, &InactiveRTV.g_pRenderTargetViewD3D11);

            D3D11_SHADER_RESOURCE_VIEW_DESC SRV;
            ZeroMemory(&SRV, sizeof(SRV));
            SRV.Format = (DXGI_FORMAT)TD.Format;
            SRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            SRV.Texture2D.MostDetailedMip = 0;
            SRV.Texture2D.MipLevels = 1;

            h = ptrDEV->CreateShaderResourceView(InactiveTexture.m_TextureD3D11, &SRV, &G_InactiveSRV);

            ///
            /// Camara activa
            ///
            CDevCont.g_pImmediateContextD3D11->OMSetRenderTargets(0, 0, 0);
            CRendTarView.g_pRenderTargetViewD3D11->Release();

            h = ptrSwap->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

            ClaseBuffer tempBack;
            h = ptrSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tempBack.m_BufferD3D11);
            h = ptrDEV->CreateRenderTargetView(tempBack.m_BufferD3D11, NULL, &CRendTarView.g_pRenderTargetViewD3D11);
            tempBack.m_BufferD3D11->Release();

            Texture2Desc DepthDesc;
            ZeroMemory(&DepthDesc, sizeof(DepthDesc));
            DepthDesc.Width = width;
            DepthDesc.Height = height;
            DepthDesc.MipLevels = 1;
            DepthDesc.ArraySize = 1;
            DepthDesc.Format = FORMAT_D24_UNORM_S8_UINT;
            DepthDesc.SampleDesc.My_Count = 1;
            DepthDesc.SampleDesc.My_Quality = 0;
            DepthDesc.Usage = USAGE_DEFAULT;
            DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            DepthDesc.CPUAccessFlags = 0;
            DepthDesc.MiscFlags = 0;

            G_DepthStencil.Init(DepthDesc);

            h = ptrDEV->CreateTexture2D(&G_DepthStencil.m_TextDescD3D11, NULL, &G_DepthStencil.m_TextureD3D11);

            DepthStencilViewDesc DSVD;
            ZeroMemory(&DSVD, sizeof(DSVD));
            DSVD.Format = CDepthStencilView.m_DepthDesc.Format;
            DSVD.ViewDimension = DSV_DIMENSION_TEXTURE2D;
            DSVD.Texture2D.My_MipSlice = 0;

            CDepthStencilView.g_pDepthStencilViewD3D11->Release();

            CDepthStencilView.Init(DSVD);

            h = ptrDEV->CreateDepthStencilView(G_DepthStencil.m_TextureD3D11, &CDepthStencilView.descDepthViewD3D11, &CDepthStencilView.g_pDepthStencilViewD3D11);

            ptrDevCont->OMSetRenderTargets(1, &CRendTarView.g_pRenderTargetViewD3D11, CDepthStencilView.g_pDepthStencilViewD3D11);

            ViewportDesc VD;
            ZeroMemory(&VD, sizeof(VD));
            VD.Width = width;
            VD.Height = height;
            VD.MinDepth = 0.f;
            VD.MaxDepth = 1.f;
            VD.TopLeftX = 0;
            VD.TopLeftY = 0;

            ClaseViewport ViewPort;
            ViewPort.Init(VD);
            ptrDevCont->RSSetViewports(1, &ViewPort.vpD3D11);
        }
    }
#endif
}

///
/// Function for level data
///
enum LevelStuff {

    Wall = 1,
    Void = 2,
    Pilares = 4,
    NotColideWalls = 5
};

///
/// Function to generate the map
///
void LevelMap(std::string FileLevelName) {

    std::ifstream File;
    File.open(FileLevelName);

    File >> Columns >> Rows;

    for (int i = 0; i < Rows; i++) {

        for (int j = 0; j < Columns; j++) {

            File >> LevelCubes[i][j];
        }
    }

    for (int i = 0; i < Rows; i++) {

        for (int j = 0; j < Columns; j++) {

            std::cout << LevelCubes[i][j] << " ";
        }
        std::cout << std::endl;
    }
    File.close();
}

///
/// Function to initialize cameras
///
void InitCameras() {

    CameraDescriptor GodCamera;
    GodCamera.s_At = { 0,0,0 };
    GodCamera.s_Eye = { 0,0,-1 };
    GodCamera.s_Up = { 0,1,0 };
    GodCamera.s_Far = 1000;
    GodCamera.s_Near = 0.01;

#ifdef D3D11
    GodCamera.s_FoV = XM_PIDIV4;
#endif

    GodCamera.s_Height = height;
    GodCamera.s_Widht = width;

    CameraDescriptor FirstCamera;
    FirstCamera.s_At = { 0,0,0 };
    FirstCamera.s_Eye = { 0,0,-6 };
    FirstCamera.s_Up = { 0,1,0 };
    FirstCamera.s_Far = 1000;
    FirstCamera.s_Near = 0.01;

#ifdef D3D11
    FirstCamera.s_FoV = XM_PIDIV4;
#endif

    FirstCamera.s_Height = height;
    FirstCamera.s_Widht = width;

    FreeCamera.Init(GodCamera);
    FPSCamera.Init(FirstCamera);

    MainCamera = &FPSCamera;
    SecondCamera = &FreeCamera;
}

///
/// Function to activate the console
///
void activateConsole()
{
    ///
    /// Create a console for this application
    ///
    AllocConsole();

    ///
    /// Get STDOUT handle
    ///
    HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
    FILE* COutputHandle = _fdopen(SystemOutput, "w");

    ///
    /// Get STDERR handle
    ///
    HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
    int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
    FILE* CErrorHandle = _fdopen(SystemError, "w");

    ///
    /// Get STDIN handle
    ///
    HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
    FILE* CInputHandle = _fdopen(SystemInput, "r");

    ///
    /// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
    ///
    std::ios::sync_with_stdio(true);

    ///
    /// Redirect the CRT standard input, output, and error handles to the console
    ///
    freopen_s(&CInputHandle, "CONIN$", "r", stdin);
    freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
    freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

    ///
    /// Clear the error state for each of the C++ standard stream objects. We need to do this, as attempts to access the standard streams before they refer to a valid target will cause the
    ///

    ///
    /// iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems to always occur during startup regardless of whether anything has been read from or written to
    ///

    ///
    /// the console or not.
    ///
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();

}

///
/// Forward declarations
///
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();



///
/// Entry point to the program. Initializes everything and goes into a message processing, loop. Idle time is used to render the scene.
///
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(InitWindow(hInstance, nCmdShow)))
        return 0;

#ifdef D3D11
    if (FAILED(InitDevice()))
    {
        CleanupDevice();
        return 0;
    }
#endif // D3D11

    ///
    /// Main message loop
    ///
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
#ifdef D3D11
            ImVec2 ScreenImGui(200, 200);

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();

            ImGui::NewFrame();
            ImGui::Begin("Change Cameras");
            if (ImGui::Button("Click"))
            {
                Camera* Temporal = SecondCamera;
                SecondCamera = MainCamera;
                MainCamera = Temporal;
            }
            ImGui::End();

            ImGui::Begin("Shader from Camera");
            ImGui::Image(G_InactiveSRV, ScreenImGui);
            ImGui::GetIO().FontGlobalScale;
            ImGui::End();
            Render();
#endif // D3D11
        }
    }

    CleanupDevice();
    LevelMap("Mapa.txt");
    return (int)msg.wParam;
}

///
/// Register class and create window
///
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow){

    activateConsole();    
    //Register class
    /*WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 800, 800 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    g_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
        NULL);
    if (!g_hWnd)
        return E_FAIL;

    ShowWindow(g_hWnd, nCmdShow);

    LevelMap("Mapa.txt");*/

    g_OpenGlObj.WindowGLFW();
    return S_OK;
}

///
/// Helper for compiling shaders with D3DX11
///
#ifdef D3D11
HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )

    ///
    /// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    ///

    ///
    /// Setting this flag improves the shader debugging experience, but still allows the shaders to be optimized and to run exactly the way they will run in 
    ///

    ///
    /// the release configuration of this program.
    ///
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ///
    /// Un buffer que va a contener el resultado del shader.
    ///
    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);

    if (FAILED(hr))
    {
        if (pErrorBlob != NULL)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}
#endif

///
/// Dentro de la interfaz y en ni desc del input layout debo pedir el blob TO
///
#ifdef D3D11
HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
{
    ///
    /// Reflect shader info
    ///
    ID3D11ShaderReflection* pVertexShaderReflection = NULL;
    if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
    {
        return S_FALSE;
    }

    ///
    /// Get shader info
    ///
    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->GetDesc(&shaderDesc);

    ///
    /// Read input layout description from shader info
    ///
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

    int offset = 0;

    for (int i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

        ///
        /// Fill out input element desc
        ///
        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = offset;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;

        ///
        /// Determine DXGI format
        ///
        if (paramDesc.Mask == 1)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (paramDesc.Mask <= 3)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (paramDesc.Mask <= 15)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; offset += 12;
        }
        else if (paramDesc.Mask <= 7)
        {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        ///
        /// save element desc
        ///
        inputLayoutDesc.push_back(elementDesc);
    }

    ///
    /// Try to create Input Layout
    ///
    HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

    ///
    /// Free allocation shader reflection memory
    ///
    pVertexShaderReflection->Release();
    return hr;
}
#endif

///
/// Create Direct3D device and swap chain
///
#ifdef D3D11
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    width = rc.right - rc.left;
    height = rc.bottom - rc.top;
    //#ifdef D3D11
    UINT createDeviceFlags = 0;
    //#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    //#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,

        ///
        /// Work in combination
        ///
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    ///
    /// Mis movimientos
    ///
    DeviceDescriptor objDev;
    objDev.g_driverType = DRIVER_TYPE_NULL;
    CDev.g_pd3dDeviceD3D11 = NULL;
    objDev.s_createDeviceFlags = 2;
    objDev.s_numFeatureLevels = 3;

    CDev.Init(objDev);

    SwapChainDescriptor objSd;
    objSd.BufferCount = 1;
    objSd.BufferDesc.My_Width = width;
    objSd.BufferDesc.My_Height = height;
    objSd.BufferDesc.My_Format = FORMAT_R8G8B8A8_UNORM;
    objSd.BufferDesc.My_RefreshRate.My_Numerator = 60;
    objSd.BufferDesc.My_RefreshRate.My_Denominator = 1;
    objSd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    objSd.OutputWindow = g_hWnd;
    objSd.SampleDesc.My_Count = 1;
    objSd.SampleDesc.My_Quality = 0;
    objSd.Windowed = TRUE;

    CSwap.Init(objSd);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        CDev.g_driverTypeD3D11 = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(NULL, CDev.g_driverTypeD3D11, NULL, objDev.s_createDeviceFlags, featureLevels, objDev.s_numFeatureLevels,
            D3D11_SDK_VERSION, &CSwap.sdD3D11, &CSwap.g_pSwapChainD3D11, &CDev.g_pd3dDeviceD3D11, &CDev.g_featureLevelD3D11, &CDevCont.g_pImmediateContextD3D11);
        if (SUCCEEDED(hr))
            break;
    }

    ptrDEV = static_cast<ID3D11Device*>(CDev.GetDev());
    ptrDevCont = static_cast<ID3D11DeviceContext*>(CDevCont.GetDevCont());
    ptrSwap = static_cast<IDXGISwapChain*>(CSwap.GetSwap());

    if (FAILED(hr))
        return hr;

    ///
    /// Create a render target view
    ///
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = CSwap.g_pSwapChainD3D11->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = CDev.g_pd3dDeviceD3D11->CreateRenderTargetView(pBackBuffer, NULL, &CRendTarView.g_pRenderTargetViewD3D11);
    pBackBuffer->Release();

    if (FAILED(hr))
        return hr;

    ///
    /// Create depth stencil texture
    ///
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    hr = CDev.g_pd3dDeviceD3D11->CreateTexture2D(&descDepth, NULL, &CRendTar.g_pDepthStencilD3D11);

    if (FAILED(hr))
        return hr;
 
    ///
    /// Create the depth stencil view
    ///
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    hr = CDev.g_pd3dDeviceD3D11->CreateDepthStencilView(CRendTar.g_pDepthStencilD3D11, &descDSV, &CDepthStencilView.g_pDepthStencilViewD3D11);

    if (FAILED(hr))
        return hr;

    CDevCont.g_pImmediateContextD3D11->OMSetRenderTargets(1, &CRendTarView.g_pRenderTargetViewD3D11, CDepthStencilView.g_pDepthStencilViewD3D11);

    ///
    /// Setup the viewport
    ///
    ViewportDesc  vp2;
    vp2.Width = (FLOAT)width;
    vp2.Height = (FLOAT)height;
    vp2.MinDepth = 0.0f;
    vp2.MaxDepth = 1.0f;
    vp2.TopLeftX = 0;
    vp2.TopLeftY = 0;
    CView.Init(vp2);

    CDevCont.g_pImmediateContextD3D11->RSSetViewports(1, &CView.vpD3D11);

    ///
    /// Compile the vertex shader
    ///
    hr = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &CShader.m_pVSBlobD3D11);
    if (FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    ///
    /// Create the vertex shader
    ///
    hr = CDev.g_pd3dDeviceD3D11->CreateVertexShader(CShader.m_pVSBlobD3D11->GetBufferPointer(), CShader.m_pVSBlobD3D11->GetBufferSize(), NULL, &CShader.g_pVertexShaderD3D11);
    if (FAILED(hr))
    {
        CShader.m_pVSBlobD3D11->Release();
        return hr;
    }

    ///
    /// Define the input layout
    ///

    ///
    /// Create input layout from compiled VS
    ///
    hr = CreateInputLayoutDescFromVertexShaderSignature(CShader.m_pVSBlobD3D11, CDev.g_pd3dDeviceD3D11, &CShader.LayoutD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// Create the input layout
    ///

    ///
    /// Set the input layout
    ///
    CDevCont.g_pImmediateContextD3D11->IASetInputLayout(CShader.LayoutD3D11);

    ///
    /// Compile the pixel shader
    ///
    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
            L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    ///
    /// Create the pixel shader
    ///
    hr = CDev.g_pd3dDeviceD3D11->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;

    ///
    /// Create vertex buffer
    ///
    SimpleVertex vertices[] =
    {
        {  mathfu::float3(-1.0f, 1.0f, -1.0f), mathfu::float2(0.0f, 0.0f) },
        {  mathfu::float3(1.0f, 1.0f, -1.0f),  mathfu::float2(1.0f, 0.0f) },
        {  mathfu::float3(1.0f, 1.0f, 1.0f),   mathfu::float2(1.0f, 1.0f) },
        {  mathfu::float3(-1.0f, 1.0f, 1.0f),  mathfu::float2(0.0f, 1.0f) },

        {  mathfu::float3(-1.0f, -1.0f, -1.0f),mathfu::float2(0.0f, 0.0f) },
        {  mathfu::float3(1.0f, -1.0f, -1.0f), mathfu::float2(1.0f, 0.0f) },
        {  mathfu::float3(1.0f, -1.0f, 1.0f),  mathfu::float2(1.0f, 1.0f) },
        {  mathfu::float3(-1.0f, -1.0f, 1.0f), mathfu::float2(0.0f, 1.0f) },

        {  mathfu::float3(-1.0f, -1.0f, 1.0f), mathfu::float2(0.0f, 0.0f) },
        {  mathfu::float3(-1.0f, -1.0f, -1.0f),mathfu::float2(1.0f, 0.0f) },
        {  mathfu::float3(-1.0f, 1.0f, -1.0f), mathfu::float2(1.0f, 1.0f) },
        {  mathfu::float3(-1.0f, 1.0f, 1.0f),  mathfu::float2(0.0f, 1.0f) },

        {  mathfu::float3(1.0f, -1.0f, 1.0f),  mathfu::float2(0.0f, 0.0f) },
        {  mathfu::float3(1.0f, -1.0f, -1.0f), mathfu::float2(1.0f, 0.0f) },
        {  mathfu::float3(1.0f, 1.0f, -1.0f),  mathfu::float2(1.0f, 1.0f) },
        {  mathfu::float3(1.0f, 1.0f, 1.0f),   mathfu::float2(0.0f, 1.0f) },

        {  mathfu::float3(-1.0f, -1.0f, -1.0f),mathfu::float2(0.0f, 0.0f) },
        {  mathfu::float3(1.0f, -1.0f, -1.0f), mathfu::float2(1.0f, 0.0f) },
        {  mathfu::float3(1.0f, 1.0f, -1.0f),  mathfu::float2(1.0f, 1.0f) },
        {  mathfu::float3(-1.0f, 1.0f, -1.0f), mathfu::float2(0.0f, 1.0f) },

        {  mathfu::float3(-1.0f, -1.0f, 1.0f), mathfu::float2(0.0f, 0.0f) },
        {  mathfu::float3(1.0f, -1.0f, 1.0f),  mathfu::float2(1.0f, 0.0f) },
        {  mathfu::float3(1.0f, 1.0f, 1.0f),   mathfu::float2(1.0f, 1.0f) },
        {  mathfu::float3(-1.0f, 1.0f, 1.0f),  mathfu::float2(0.0f, 1.0f) },
    };

    BufferDescriptor bd2;
    ZeroMemory(&bd2, sizeof(bd2));
    bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(SimpleVertex) * 24;
    bd2.BindFlags = BIND_VERTEX_BUFFER;
    bd2.CPUAccessFlags = 0;

    SUBRESOURCE_DATA initData2;
    ZeroMemory(&initData2, sizeof(initData2));
    initData2.My_pSysMem = vertices;

    CVertexBuffer.Init(initData2, bd2);

    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&CVertexBuffer.m_Buffer.m_BufferDescD3D11, &CVertexBuffer.m_SubDataD3D11, &CVertexBuffer.m_Buffer.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// Set vertex buffer
    ///
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    CDevCont.g_pImmediateContextD3D11->IASetVertexBuffers(0, 1, &CVertexBuffer.m_Buffer.m_BufferD3D11, &stride, &offset);

    ///
    /// Create index buffer
    ///
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(WORD) * 36;
    bd2.BindFlags = BIND_INDEX_BUFFER;
    bd2.CPUAccessFlags = 0;
    initData2.My_pSysMem = indices;

    CIndexBuffer.Init(initData2, bd2);

    ///
    /// Set index buffer
    ///
    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&CIndexBuffer.m_Buffer.m_BufferDescD3D11, &CIndexBuffer.m_SubDataD3D11, &CIndexBuffer.m_Buffer.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// Set index bufferUINT stride = sizeof(SimpleVertex);
    ///
    CDevCont.g_pImmediateContextD3D11->IASetIndexBuffer(CIndexBuffer.m_Buffer.m_BufferD3D11, DXGI_FORMAT_R16_UINT, 0);
 
    ///
    /// Set primitive topology
    ///
    CDevCont.g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
    ///
    /// Create the constant buffers
    ///
    bd2.Usage = USAGE_DEFAULT;
    bd2.ByteWidth = sizeof(CBNeverChanges);
    bd2.BindFlags = BIND_CONSTANT_BUFFER;
    bd2.CPUAccessFlags = 0;
    FreeCamera.g_pCBNeverChangesCamera.Init(bd2);

    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&FreeCamera.g_pCBNeverChangesCamera.m_BufferDescD3D11, NULL, &FreeCamera.g_pCBNeverChangesCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// Inicialización NeverChange FirstPersonCamera
    ///
    FPSCamera.g_pCBNeverChangesCamera.Init(bd2);
    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&FPSCamera.g_pCBNeverChangesCamera.m_BufferDescD3D11, NULL, &FPSCamera.g_pCBNeverChangesCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// bd.ByteWidth = sizeof(CBChangeOnResize);
    ///
    bd2.ByteWidth = sizeof(CBChangeOnResize);
    FreeCamera.g_pCBChangeOnResizeCamera.Init(bd2);
    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&FreeCamera.g_pCBChangeOnResizeCamera.m_BufferDescD3D11, NULL, &FreeCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// Inicialización Resize FirstPersonCamera
    ///
    FPSCamera.g_pCBChangeOnResizeCamera.Init(bd2);
    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&FPSCamera.g_pCBChangeOnResizeCamera.m_BufferDescD3D11, NULL, &FPSCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// bd.ByteWidth = sizeof(CBChangesEveryFrame);
    ///
    bd2.ByteWidth = sizeof(CBChangesEveryFrame);
    FreeCamera.g_pCBChangesEveryFrameCamera.Init(bd2);
    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&FreeCamera.g_pCBChangesEveryFrameCamera.m_BufferDescD3D11, NULL, &FreeCamera.g_pCBChangesEveryFrameCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// Inicialización ChangeEveryFrame FirstPersonCamera
    ///
    FPSCamera.g_pCBChangesEveryFrameCamera.Init(bd2);
    hr = CDev.g_pd3dDeviceD3D11->CreateBuffer(&FPSCamera.g_pCBChangesEveryFrameCamera.m_BufferDescD3D11, NULL, &FPSCamera.g_pCBChangesEveryFrameCamera.m_BufferD3D11);
    if (FAILED(hr))
        return hr;

    ///
    /// Load the Texture
    ///
    hr = D3DX11CreateShaderResourceViewFromFile(CDev.g_pd3dDeviceD3D11, L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
    if (FAILED(hr))
        return hr;
 
    ///
    /// Create the sample state
    ///
    SampleStateDesc sampDesc2;
    //(UN POSIBLE ZERO MEMORY)
    //ZeroMemory(&sampDesc2, sizeof(sampDesc2));
    sampDesc2.Filter = FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc2.AddressU = TEXTURE_ADDRESS_WRAP;
    sampDesc2.AddressV = TEXTURE_ADDRESS_WRAP;
    sampDesc2.AddressW = TEXTURE_ADDRESS_WRAP;
    sampDesc2.ComparisonFunc = COMPARISON_NEVER;
    sampDesc2.MinLOD = 0;
    sampDesc2.MaxLOD = D3D11_FLOAT32_MAX;
    CSampleS.Init(sampDesc2);

    hr = CDev.g_pd3dDeviceD3D11->CreateSamplerState(&CSampleS.sampDescD3D11, &CSampleS.g_pSamplerLinearD3D11);
    if (FAILED(hr))
        return hr;

    //#endif
    ///
    /// Initialize the world matrices
    ///
    g_World = g_World.Identity();
 
    ///
    /// Initialize the view matrix
    ///
    mathfu::Vector<float, 4> Eye(0.0f, 3.0f, -6.0f, 0.0f);
    mathfu::Vector<float, 4> At(0.0f, 1.0f, 0.0f, 0.0f);
    mathfu::Vector<float, 4> Up(0.0f, 1.0f, 0.0f, 0.0f);

    // g_View = XMMatrixLookAtLH( Eye, At, Up );

    InitCameras();

    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = FPSCamera.GetView();

    //#ifdef D3D11
    CDevCont.g_pImmediateContextD3D11->UpdateSubresource(FPSCamera.g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cbNeverChanges, 0, 0);
    //#endif

    cbNeverChanges.mView = FreeCamera.GetView();

    //#ifdef D3D11
    CDevCont.g_pImmediateContextD3D11->UpdateSubresource(FreeCamera.g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cbNeverChanges, 0, 0);
    //#endif

    ///
    /// Initialize the projection matrix
    ///
    CBChangeOnResize cbChangesOnResize;
    cbChangesOnResize.mProjection = FPSCamera.GetProjection();
    //#ifdef D3D11
    CDevCont.g_pImmediateContextD3D11->UpdateSubresource(FPSCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize, 0, 0);
    //#endif

    cbChangesOnResize.mProjection = FreeCamera.GetProjection();

    //#ifdef D3D11
    CDevCont.g_pImmediateContextD3D11->UpdateSubresource(FreeCamera.g_pCBChangeOnResizeCamera.m_BufferD3D11, 0, NULL, &cbChangesOnResize, 0, 0);
    //#endif

    Texture2Desc T;
    ZeroMemory(&T, sizeof(T));
    T.Width = width;
    T.Height = height;
    T.MipLevels = T.ArraySize = 1;
    T.Format = FORMAT_R8G8B8A8_UNORM;
    T.SampleDesc.My_Count = 1;
    T.Usage = USAGE_DEFAULT;
    T.BindFlags = BIND_SHADER_RESOURCE | BIND_RENDER_TARGET;
    //#ifdef D3D11
    T.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //#endif
    T.MiscFlags = 0;

    InactiveTexture.Init(T);

    //#ifdef D3D11
    hr = CDev.g_pd3dDeviceD3D11->CreateTexture2D(&InactiveTexture.m_TextDescD3D11, NULL, &InactiveTexture.m_TextureD3D11);
    //#endif
    if (FAILED(hr)) {

        return hr;
    }

    RenderTargetViewDesc RTVDesc;
    ZeroMemory(&RTVDesc, sizeof(RTVDesc));
    RTVDesc.Format = T.Format;
    RTVDesc.ViewDimension = RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.My_MipSlice = 0;

    InactiveRTV.Init(RTVDesc);

    //#ifdef D3D11
    hr = CDev.g_pd3dDeviceD3D11->CreateRenderTargetView(InactiveTexture.m_TextureD3D11, &InactiveRTV.m_renderTVD3D11, &InactiveRTV.g_pRenderTargetViewD3D11);
    //#endif
    if (FAILED(hr)) {

        return hr;
    }

    //#ifdef D3D11
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    SRVDesc.Format = (DXGI_FORMAT)T.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = 1;
    //#endif

    //#ifdef D3D11
    hr = CDev.g_pd3dDeviceD3D11->CreateShaderResourceView(InactiveTexture.m_TextureD3D11, &SRVDesc, &G_InactiveSRV);
    //#endif
    if (FAILED(hr)) {

        return hr;
    }

    //#ifdef D3D11
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(CDev.g_pd3dDeviceD3D11, CDevCont.g_pImmediateContextD3D11);
    ImGui::StyleColorsDark();
    //#endif
    InitCameras();

    ///
    /// API GRAPHIC
    ///
    //G_GraphicApi.ChargeMesh("noivern/Noivern.fbx", &G_SceneManager, G_GraphicApi.m_Model, CDevCont, G_GraphicApi.m_Importer, &CDev);
    G_GraphicApi.ChargeMesh("ugandan-knuckles/source/Knuckles.fbx", &G_SceneManager, G_GraphicApi.m_Model, CDevCont, G_GraphicApi.m_Importer, &CDev);

    return S_OK;
}
#endif // D3D11

///
/// Clean up the objects we've created
///
void CleanupDevice()
{
#ifdef D3D11
    if (CDevCont.g_pImmediateContextD3D11) CDevCont.g_pImmediateContextD3D11->ClearState();
    if (CSampleS.g_pSamplerLinearD3D11) CSampleS.g_pSamplerLinearD3D11->Release();
    if (g_pTextureRV) g_pTextureRV->Release();
    if (FCNeverChange.m_BufferD3D11)FCNeverChange.m_BufferD3D11->Release();
    if (FCOnResize.m_BufferD3D11)FCOnResize.m_BufferD3D11->Release();
    if (FCChangeEveryFrame.m_BufferD3D11)FCChangeEveryFrame.m_BufferD3D11->Release();
    if (CVertexBuffer.m_Buffer.m_BufferD3D11)CVertexBuffer.m_Buffer.m_BufferD3D11->Release();
    if (CIndexBuffer.m_Buffer.m_BufferD3D11)CIndexBuffer.m_Buffer.m_BufferD3D11->Release();
    if (CShader.LayoutD3D11) CShader.LayoutD3D11->Release();
    if (CShader.g_pVertexShaderD3D11) CShader.g_pVertexShaderD3D11->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    if (CRendTar.g_pDepthStencilD3D11) CRendTar.g_pDepthStencilD3D11->Release();
    if (CDepthStencilView.g_pDepthStencilViewD3D11) CDepthStencilView.g_pDepthStencilViewD3D11->Release();
    if (CRendTarView.g_pRenderTargetViewD3D11) CRendTarView.g_pRenderTargetViewD3D11->Release();
    if (CSwap.g_pSwapChainD3D11) CSwap.g_pSwapChainD3D11->Release();
    if (CDevCont.g_pImmediateContextD3D11) CDevCont.g_pImmediateContextD3D11->Release();
    if (CDev.g_pd3dDeviceD3D11) CDev.g_pd3dDeviceD3D11->Release();
    if (FPCNeverChange.m_BufferD3D11)FPCNeverChange.m_BufferD3D11->Release();
    if (FPCOnResize.m_BufferD3D11)FPCOnResize.m_BufferD3D11->Release();
    if (FPCChangeEveryFrame.m_BufferD3D11)FPCChangeEveryFrame.m_BufferD3D11->Release();
    if (InactiveTexture.m_TextureD3D11)  InactiveTexture.m_TextureD3D11->Release();
    if (InactiveRTV.g_pRenderTargetViewD3D11) InactiveRTV.g_pRenderTargetViewD3D11->Release();
    if (G_InactiveSRV)G_InactiveSRV->Release();
#endif
}

///
/// Called every time the application receives a message
///
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    if (io.WantCaptureMouse){

        ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
        return true;
    }

    PAINTSTRUCT ps;
    HDC hdc;
    POINT Temp;

    switch (message)
    {
        case WM_PAINT:

            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:

            PostQuitMessage(0);
            break;

        ///
        /// WM_SIZE: It is sent to a window after its size has changed.
        ///
        case WM_SIZE: {

            Resize();
            break;
        }

        ///
        /// WM_KEYDOWN: A window receives keyboard input in the form of keystroke messages and character messages.
        ///
        case WM_KEYDOWN: {

            MainCamera->inputs(wParam);

            CBNeverChanges cb;
            cb.mView = MainCamera->GetView();

#if defined(D3D11)
            CDevCont.g_pImmediateContextD3D11->UpdateSubresource(MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
#endif
            break;
        }

        ///
        /// WM_LBUTTONDOWN: Posted when the user presses the left mouse button while the cursor is in the client area of a window.
        ///
        case WM_LBUTTONDOWN:

            GetCursorPos(&Temp);
            //MainCamera->SetOriginalMousePos(Temp.x, Temp.y);c
            MainCamera->OriginalMousePos = mathfu::float2(Temp.x, Temp.y);
            MainCamera->m_ClickPressed = true;
            break;

        case WM_MOUSEMOVE:

#ifdef D3D11
            if (MainCamera->m_ClickPressed)
            {
                MainCamera->SetOriginalMousePos(MainCamera->OriginalMousePos.x, MainCamera->OriginalMousePos.y);
                MainCamera->MouseRotation();
                CBNeverChanges cb;
                cb.mView = MainCamera->GetView();
                CDevCont.g_pImmediateContextD3D11->UpdateSubresource(MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
            }
#endif // D3D11            
            break;

        ///
        /// WM_LBUTTONUP: Posted when the user releases the left mouse button while the cursor is in the client area of a window.
        ///
        case WM_LBUTTONUP:

            MainCamera->m_ClickPressed = false;
            break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

///
/// Render a frame
///
#ifdef D3D11
void Render()
{
    G_SceneManager.Update(); 
    
    ///
    /// Update our time
    ///
    static float t = 0.0f;
    if (CDev.m_DescDevice.g_driverType == D3D_DRIVER_TYPE_REFERENCE){

        t += (float)XM_PI * 0.0125f;
    }
    else{

        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    g_World = g_World.FromTranslationVector(mathfu::Vector<float, 3>(3.0f, 0.0f, 0.0f));

    ///
    /// Modify the color
    ///
    g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

    ///
    /// Clear the back buffer
    ///
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

    CDevCont.g_pImmediateContextD3D11->OMSetRenderTargets       (1, &InactiveRTV.g_pRenderTargetViewD3D11, CDepthStencilView.g_pDepthStencilViewD3D11);
    CDevCont.g_pImmediateContextD3D11->ClearRenderTargetView    (InactiveRTV.g_pRenderTargetViewD3D11, ClearColor);
    
    ///
    /// Clear the depth buffer to 1.0 (max depth)
    ///
    CDevCont.g_pImmediateContextD3D11->ClearDepthStencilView    (CDepthStencilView.g_pDepthStencilViewD3D11, D3D11_CLEAR_DEPTH, 1.0f, 0);

    ///
    /// Update variables that change once per frame
    ///

    ///
    /// Render the cube
    ///
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    CDevCont.g_pImmediateContextD3D11->IASetVertexBuffers     (0, 1, &CVertexBuffer.m_Buffer.m_BufferD3D11, &stride, &offset);
    CDevCont.g_pImmediateContextD3D11->IASetIndexBuffer       (CIndexBuffer.m_Buffer.m_BufferD3D11, DXGI_FORMAT_R16_UINT, 0);
    CDevCont.g_pImmediateContextD3D11->VSSetShader            (CShader.g_pVertexShaderD3D11, NULL, 0);
    CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers   (0, 1, &MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11);
    CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers   (1, 1, &MainCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11);
    CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers   (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
    CDevCont.g_pImmediateContextD3D11->PSSetShader            (g_pPixelShader, NULL, 0);
    CDevCont.g_pImmediateContextD3D11->PSSetConstantBuffers   (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
    CDevCont.g_pImmediateContextD3D11->PSSetShaderResources   (0, 1, &g_pTextureRV);
    CDevCont.g_pImmediateContextD3D11->PSSetSamplers          (0, 1, &CSampleS.g_pSamplerLinearD3D11);
    //CDevCont.g_pImmediateContextD3D11->DrawIndexed            (36, 0, 0);

    int DistanceX = 0;
    int DistanceY = 0;
    for (int i = 0; i < Rows; i++){

        for (int j = 0; j < Columns; j++){

            if (LevelCubes[i][j]){

                DistanceX += 2.5;
            }
            else if (LevelCubes[i][j] == Pilares){

                DistanceX += 2.5;
            }
            else{

                DistanceX += 2.5;
            }
            if (LevelCubes[i][j] != 0){

                CBChangesEveryFrame cb;
                cb.mWorld = g_World.Transpose();
                cb.vMeshColor = g_vMeshColor;

                CDevCont.g_pImmediateContextD3D11->UpdateSubresource      (SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
                CDevCont.g_pImmediateContextD3D11->VSSetShader            (CShader.g_pVertexShaderD3D11, NULL, 0);
                CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers   (0, 1, &SecondCamera->g_pCBNeverChangesCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers   (1, 1, &SecondCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers   (2, 1, &SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->PSSetShader            (g_pPixelShader, NULL, 0);
                CDevCont.g_pImmediateContextD3D11->PSSetConstantBuffers   (2, 1, &SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->PSSetShaderResources   (0, 1, &g_pTextureRV);
                CDevCont.g_pImmediateContextD3D11->PSSetSamplers          (0, 1, &CSampleS.g_pSamplerLinearD3D11);
                CDevCont.g_pImmediateContextD3D11->DrawIndexed            (36, 0, 0);
            }
            g_World = g_World.FromTranslationVector(mathfu::Vector<float, 3>(DistanceX, 0.0f, DistanceY));
        }
        DistanceY += 2.5;
        DistanceX = 0;
    }

    CBChangesEveryFrame m_MeshData;
    m_MeshData.mWorld = {
        1,0,0,MainCamera->GetEye().x,
        0,1,0,MainCamera->GetEye().y,
        0,0,1,MainCamera->GetEye().z,
       0,0,0,1
    };

    m_MeshData.vMeshColor = { 1,1,1,1 };
    CDevCont.g_pImmediateContextD3D11->UpdateSubresource(SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &m_MeshData, 0, 0);
    CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &SecondCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);

    for (size_t i = 0; i < G_SceneManager.m_MeshInScene.size(); i++)
    {
         CDevCont.g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &G_SceneManager.m_MeshInScene[i]->m_Materials->m_TexDif);
         UINT stride = sizeof(SimpleVertex);
         UINT offset = 0;
    
         CDevCont.g_pImmediateContextD3D11->IASetVertexBuffers
         (
             0,

             ///
             /// number of buffers we are using
             ///
             1,

             ///
             /// pointer to the buffers list
             ///
             &G_SceneManager.m_MeshInScene[i]->m_VertexBuffer->m_BufferD3D11,

             ///
             /// a uint indicating the size of a single vertex
             ///
             &stride,
             &offset
         );//un uint que indica el numero del byte en el vertice del que se quiere comenzar a pintar
    
         CDevCont.g_pImmediateContextD3D11->IASetIndexBuffer
         (
             G_SceneManager.m_MeshInScene[i]->m_Index->m_BufferD3D11,
             DXGI_FORMAT_R16_UINT,
             0
         );
    
         ///
         /// Tipo de topologia
         ///

         ///
         /// This second function tells Direct3D what type of primitive is used.
         ///

         ///
         /// _devCont.g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
         ///

         ///
         /// Draws the vertex buffer in the back buffer
         ///
         CDevCont.g_pImmediateContextD3D11->DrawIndexed(G_SceneManager.m_MeshInScene[i]->m_IndexNum, 0, 0);
    }
    CDevCont.g_pImmediateContextD3D11->OMSetRenderTargets   (1, &CRendTarView.g_pRenderTargetViewD3D11, CDepthStencilView.g_pDepthStencilViewD3D11);
    CDevCont.g_pImmediateContextD3D11->ClearRenderTargetView(CRendTarView.g_pRenderTargetViewD3D11, ClearColor);
    CDevCont.g_pImmediateContextD3D11->ClearDepthStencilView(CDepthStencilView.g_pDepthStencilViewD3D11, D3D11_CLEAR_DEPTH, 1.0f, 0);
 
    CDevCont.g_pImmediateContextD3D11->IASetVertexBuffers(0, 1, &CVertexBuffer.m_Buffer.m_BufferD3D11, &stride, &offset);
    CDevCont.g_pImmediateContextD3D11->IASetIndexBuffer(CIndexBuffer.m_Buffer.m_BufferD3D11, DXGI_FORMAT_R16_UINT, 0);

    ///
    /// Render the cube
    ///
    DistanceX = 0;
    DistanceY = 0;

    for (int i = 0; i < Rows; i++){

        for (int j = 0; j < Columns; j++){

            if (LevelCubes[i][j]){

                DistanceX += 2.5;
            }
            else if (LevelCubes[i][j] == Pilares){

                DistanceX += 2.5;
            }
            else{

                DistanceX += 2.5;
            }
            if (LevelCubes[i][j] != 0){

                CBChangesEveryFrame cb;
                cb.mWorld = g_World.Transpose();
                cb.vMeshColor = g_vMeshColor;
                
                CDevCont.g_pImmediateContextD3D11->UpdateSubresource    (MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &cb, 0, 0);
                CDevCont.g_pImmediateContextD3D11->VSSetShader          (CShader.g_pVertexShaderD3D11, NULL, 0);
                CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers (0, 1, &MainCamera->g_pCBNeverChangesCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers (1, 1, &MainCamera->g_pCBChangeOnResizeCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->PSSetShader          (g_pPixelShader, NULL, 0);
                CDevCont.g_pImmediateContextD3D11->PSSetConstantBuffers (2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);
                CDevCont.g_pImmediateContextD3D11->PSSetShaderResources (0, 1, &G_InactiveSRV);
                CDevCont.g_pImmediateContextD3D11->PSSetSamplers        (0, 1, &CSampleS.g_pSamplerLinearD3D11);
                CDevCont.g_pImmediateContextD3D11->DrawIndexed          (36, 0, 0);
            }
            g_World = g_World.FromTranslationVector(mathfu::Vector<float, 3>(DistanceX, 0.0f, DistanceY));
        }   
        DistanceY += 2.5;
        DistanceX = 0;
    }

    m_MeshData.mWorld = {
        1,0,0,SecondCamera->GetEye().x,
        0,1,0,SecondCamera->GetEye().y,
        0,0,1,SecondCamera->GetEye().z,
       0,0,0,1
    };

    m_MeshData.vMeshColor = { 1,1,1,1 };
    CDevCont.g_pImmediateContextD3D11->UpdateSubresource(MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11, 0, NULL, &m_MeshData, 0, 0);
    CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &MainCamera->g_pCBChangesEveryFrameCamera.m_BufferD3D11);

    for (size_t i = 0; i < G_SceneManager.m_MeshInScene.size(); i++)
    {
        CDevCont.g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &G_SceneManager.m_MeshInScene[i]->m_Materials->m_TexDif);
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;

        CDevCont.g_pImmediateContextD3D11->IASetVertexBuffers
        (
            0,
            ///
            /// number of buffers we are using
            ///
            1,

            ///
            /// pointer to the buffers list
            ///
            &G_SceneManager.m_MeshInScene[i]->m_VertexBuffer->m_BufferD3D11,

            ///
            /// a uint indicating the size of a single vertex
            ///
            &stride,
            &offset
        );//un uint que indica el numero del byte en el vertice del que se quiere comenzar a pintar

        CDevCont.g_pImmediateContextD3D11->IASetIndexBuffer
        (
            G_SceneManager.m_MeshInScene[i]->m_Index->m_BufferD3D11,
            DXGI_FORMAT_R16_UINT,
            0
        );

        ///
        /// Tipo de topologia
        ///
       
        ///
        /// This second function tells Direct3D what type of primitive is used.
        ///

        ///
        /// _devCont.g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ///

        ///
        /// Draws the vertex buffer in the back buffer
        ///
        CDevCont.g_pImmediateContextD3D11->DrawIndexed(G_SceneManager.m_MeshInScene[i]->m_IndexNum, 0, 0);
    }

    //G_SceneManager.Render(&CDevCont, &SecondCamera->g_pCBChangesEveryFrameCamera, &CDev);

    ///
    /// Present our back buffer to our front buffer
    ///
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ///
    /// Present our back buffer to our front buffer
    ///
    CSwap.g_pSwapChainD3D11->Present(0, 0);
}
#endif