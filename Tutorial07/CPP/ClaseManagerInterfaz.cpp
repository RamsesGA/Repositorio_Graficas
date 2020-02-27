/*#include "..\Encabezados\ClaseManagerInterfaz.h"
#include <xnamath.h>
#include <vector>
#include "../resource.h"

HRESULT ClaseManagerInterfaz::Init(){

    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(m_hWnd, &rc);
    m_width = rc.right - rc.left;
    m_height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,//Trabaja de manera combinada
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

    DeviceDescriptor objDev;
    objDev.g_driverType = DRIVER_TYPE_NULL;
    
    m_CDev.g_pd3dDeviceD3D11 = NULL;
    
    objDev.s_createDeviceFlags = 2;
    objDev.s_numFeatureLevels = 3;

    m_CDev.Init(objDev);

    SwapChainDescriptor objSd;
    objSd.BufferCount                               = 1;
    objSd.BufferDesc.My_Width                       = m_width;
    objSd.BufferDesc.My_Height                      = m_height;
    objSd.BufferDesc.My_Format                      = FORMAT_R8G8B8A8_UNORM;
    objSd.BufferDesc.My_RefreshRate.My_Numerator    = 60;
    objSd.BufferDesc.My_RefreshRate.My_Denominator  = 1;
    objSd.BufferUsage                               = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    objSd.OutputWindow                              = m_hWnd;
    objSd.SampleDesc.My_Count                       = 1;
    objSd.SampleDesc.My_Quality                     = 0;
    objSd.Windowed                                  = TRUE;
    m_CSwap.Init(objSd);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++){

        m_CDev.g_driverTypeD3D11 = driverTypes[driverTypeIndex];
        
        hr = D3D11CreateDeviceAndSwapChain(NULL, m_CDev.g_driverTypeD3D11, NULL, objDev.s_createDeviceFlags, featureLevels, objDev.s_numFeatureLevels,
            D3D11_SDK_VERSION, &m_CSwap.sdD3D11, &m_CSwap.g_pSwapChainD3D11, &m_CDev.g_pd3dDeviceD3D11, &m_CDev.g_featureLevelD3D11, &m_CDevCont.g_pImmediateContextD3D11);
        
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr)){

        return hr;
    }

    ID3D11Texture2D* pBackBuffer = NULL;
    hr = m_CSwap.g_pSwapChainD3D11->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr)){

        return hr;
    }

    hr = m_CDev.g_pd3dDeviceD3D11->CreateRenderTargetView(pBackBuffer, NULL, &m_CRendTarView.g_pRenderTargetViewD3D11);
    pBackBuffer->Release();

    if (FAILED(hr)){

        return hr;
    }

    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory(&descDepth, sizeof(descDepth));

    descDepth.Width                 = m_width;
    descDepth.Height                = m_height;
    descDepth.MipLevels             = 1;
    descDepth.ArraySize             = 1;
    descDepth.Format                = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count      = 1;
    descDepth.SampleDesc.Quality    = 0;
    descDepth.Usage                 = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags             = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags        = 0;
    descDepth.MiscFlags             = 0;

    hr = m_CDev.g_pd3dDeviceD3D11->CreateTexture2D(&descDepth, NULL, &m_CRendTar.g_pDepthStencilD3D11);

    if (FAILED(hr)){

        return hr;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(descDSV));

    descDSV.Format              = descDepth.Format;
    descDSV.ViewDimension       = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice  = 0;

    hr = m_CDev.g_pd3dDeviceD3D11->CreateDepthStencilView(m_CRendTar.g_pDepthStencilD3D11, &descDSV, &m_CDepthS.g_pDepthStencilViewD3D11);

    if (FAILED(hr)){

        return hr;
    }

    m_CDevCont.g_pImmediateContextD3D11->OMSetRenderTargets(1, &m_CRendTarView.g_pRenderTargetViewD3D11, m_CDepthS.g_pDepthStencilViewD3D11);

    ViewportDesc  vp2;
    vp2.Width       = (FLOAT)m_width;
    vp2.Height      = (FLOAT)m_height;
    vp2.MinDepth    = 0.0f;
    vp2.MaxDepth    = 1.0f;
    vp2.TopLeftX    = 0;
    vp2.TopLeftY    = 0;
    m_CView.Init(vp2);

    m_CDevCont.g_pImmediateContextD3D11->RSSetViewports(1, &m_CView.vpD3D11);

    hr = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &m_CShader.m_pVSBlobD3D11);
    
    if (FAILED(hr)){

        MessageBox(NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = m_CDev.g_pd3dDeviceD3D11->CreateVertexShader(m_CShader.m_pVSBlobD3D11->GetBufferPointer(), m_CShader.m_pVSBlobD3D11->GetBufferSize(), NULL, &m_CShader.g_pVertexShaderD3D11);
    
    if (FAILED(hr)){

        m_CShader.m_pVSBlobD3D11->Release();
        return hr;
    }

    hr = CreateInputLayoutDescFromVertexShaderSignature(m_CShader.m_pVSBlobD3D11, m_CDev.g_pd3dDeviceD3D11, &m_CShader.LayoutD3D11);
    
    if (FAILED(hr)){

        return hr;
    }

    m_CDevCont.g_pImmediateContextD3D11->IASetInputLayout(m_CShader.LayoutD3D11);

    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
    
    if (FAILED(hr)){

        MessageBox(NULL, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

    hr = m_CDev.g_pd3dDeviceD3D11->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
    pPSBlob->Release();
   
    if (FAILED(hr)){

        return hr;
    }

    SimpleVertex vertices[] =
    {
        { mathfu::float3(-1.0f, 1.0f, -1.0f),   mathfu::float2(0.0f, 0.0f) },
        { mathfu::float3(1.0f, 1.0f, -1.0f),    mathfu::float2(1.0f, 0.0f) },
        { mathfu::float3(1.0f, 1.0f, 1.0f),     mathfu::float2(1.0f, 1.0f) },
        { mathfu::float3(-1.0f, 1.0f, 1.0f),    mathfu::float2(0.0f, 1.0f) },
        { mathfu::float3(-1.0f, -1.0f, -1.0f),  mathfu::float2(0.0f, 0.0f) },
        { mathfu::float3(1.0f, -1.0f, -1.0f),   mathfu::float2(1.0f, 0.0f) },
        { mathfu::float3(1.0f, -1.0f, 1.0f),    mathfu::float2(1.0f, 1.0f) },
        { mathfu::float3(-1.0f, -1.0f, 1.0f),   mathfu::float2(0.0f, 1.0f) },
        { mathfu::float3(-1.0f, -1.0f, 1.0f),   mathfu::float2(0.0f, 0.0f) },
        { mathfu::float3(-1.0f, -1.0f, -1.0f),  mathfu::float2(1.0f, 0.0f) },
        { mathfu::float3(-1.0f, 1.0f, -1.0f),   mathfu::float2(1.0f, 1.0f) },
        { mathfu::float3(-1.0f, 1.0f, 1.0f),    mathfu::float2(0.0f, 1.0f) },
        { mathfu::float3(1.0f, -1.0f, 1.0f),    mathfu::float2(0.0f, 0.0f) },
        { mathfu::float3(1.0f, -1.0f, -1.0f),   mathfu::float2(1.0f, 0.0f) },
        { mathfu::float3(1.0f, 1.0f, -1.0f),    mathfu::float2(1.0f, 1.0f) },
        { mathfu::float3(1.0f, 1.0f, 1.0f),     mathfu::float2(0.0f, 1.0f) },
        { mathfu::float3(-1.0f, -1.0f, -1.0f),  mathfu::float2(0.0f, 0.0f) },
        { mathfu::float3(1.0f, -1.0f, -1.0f),   mathfu::float2(1.0f, 0.0f) },
        { mathfu::float3(1.0f, 1.0f, -1.0f),    mathfu::float2(1.0f, 1.0f) },
        { mathfu::float3(-1.0f, 1.0f, -1.0f),   mathfu::float2(0.0f, 1.0f) },
        { mathfu::float3(-1.0f, -1.0f, 1.0f),   mathfu::float2(0.0f, 0.0f) },
        { mathfu::float3(1.0f, -1.0f, 1.0f),    mathfu::float2(1.0f, 0.0f) },
        { mathfu::float3(1.0f, 1.0f, 1.0f),     mathfu::float2(1.0f, 1.0f) },
        { mathfu::float3(-1.0f, 1.0f, 1.0f),    mathfu::float2(0.0f, 1.0f) },
    };

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage                = D3D11_USAGE_DEFAULT;
    bd.ByteWidth            = sizeof(SimpleVertex) * 24;
    bd.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags       = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));

    InitData.pSysMem        = vertices;
    hr                      = m_CDev.g_pd3dDeviceD3D11->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
    
    if (FAILED(hr)){

        return hr;
    }

    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;

    m_CDevCont.g_pImmediateContextD3D11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

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

    bd.Usage            = D3D11_USAGE_DEFAULT;
    bd.ByteWidth        = sizeof(WORD) * 36;
    bd.BindFlags        = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags   = 0;
    InitData.pSysMem    = indices;
    hr                  = m_CDev.g_pd3dDeviceD3D11->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
    
    if (FAILED(hr)){

        return hr;
    }

    m_CDevCont.g_pImmediateContextD3D11->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    m_CDevCont.g_pImmediateContextD3D11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    bd.Usage            = D3D11_USAGE_DEFAULT;
    bd.ByteWidth        = sizeof(CBNeverChanges);
    bd.BindFlags        = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags   = 0;
    hr                  = m_CDev.g_pd3dDeviceD3D11->CreateBuffer(&bd, NULL, &m_pCBNeverChanges);
    
    if (FAILED(hr)){

        return hr;
    }

    bd.ByteWidth    = sizeof(CBChangeOnResize);
    hr              = m_CDev.g_pd3dDeviceD3D11->CreateBuffer(&bd, NULL, &m_pCBChangeOnResize);
    
    if (FAILED(hr)){

        return hr;
    }

    bd.ByteWidth    = sizeof(CBChangesEveryFrame);
    hr              = m_CDev.g_pd3dDeviceD3D11->CreateBuffer(&bd, NULL, &m_pCBChangesEveryFrame);
    
    if (FAILED(hr)){

        return hr;
    }

    hr = D3DX11CreateShaderResourceViewFromFile(m_CDev.g_pd3dDeviceD3D11, L"seafloor.dds", NULL, NULL, &m_pTextureRV, NULL);
    
    if (FAILED(hr)){

        return hr;
    }

    SampleStateDesc sampDesc2;
    sampDesc2.Filter            = FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc2.AddressU          = TEXTURE_ADDRESS_WRAP;
    sampDesc2.AddressV          = TEXTURE_ADDRESS_WRAP;
    sampDesc2.AddressW          = TEXTURE_ADDRESS_WRAP;
    sampDesc2.ComparisonFunc    = COMPARISON_NEVER;
    sampDesc2.MinLOD            = 0;
    sampDesc2.MaxLOD            = D3D11_FLOAT32_MAX;
    m_CSampleS.Init(sampDesc2);

    hr = m_CDev.g_pd3dDeviceD3D11->CreateSamplerState(&m_CSampleS.sampDescD3D11, &m_CSampleS.g_pSamplerLinearD3D11);
    
    if (FAILED(hr)) {
    
        return hr;
    }

    m_World = XMMatrixIdentity();

    XMVECTOR Eye    = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
    XMVECTOR Up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR At     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //InitCameras();

    CBNeverChanges cbNeverChanges;

    //cbNeverChanges.mView = QuieroDormir2_Camara.GetView();

    m_CDevCont.g_pImmediateContextD3D11->UpdateSubresource(m_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

    CBChangeOnResize cbChangesOnResize;

    //cbChangesOnResize.mProjection = QuieroDormir2_Camara.GetProjection();
    m_CDevCont.g_pImmediateContextD3D11->UpdateSubresource(m_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

    return S_OK;
}

void ClaseManagerInterfaz::Update(){

    while (WM_QUIT != m_msg.message) {

        if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE)) {

            TranslateMessage(&m_msg);
            DispatchMessage(&m_msg);
        }
        else {

            Render();
        }
    }
}

void ClaseManagerInterfaz::Render(){

    int MapX = 0;
    int MapY = 0;
    
    if (m_ClickPressed && m_SwitchCamera == -1) {//If we are in free camera 

        m_QuieroDormir_Camara.MouseRotation();
    }
    else if (m_ClickPressed && m_SwitchCamera == 1) {

        m_QuieroDormir2_Camara.MouseRotation();
    }

    static float t = 0.0f;
    if (m_CDev.m_DescDevice.g_driverType == D3D_DRIVER_TYPE_REFERENCE){

        t += (float)XM_PI * 0.0125f;
    }
    else{

        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount64();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    m_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
    m_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
    m_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    
    m_CDevCont.g_pImmediateContextD3D11->ClearRenderTargetView(m_CRendTarView.g_pRenderTargetViewD3D11, ClearColor);
    m_CDevCont.g_pImmediateContextD3D11->ClearDepthStencilView(m_CDepthS.g_pDepthStencilViewD3D11, D3D11_CLEAR_DEPTH, 1.0f, 0);
    
    CBChangesEveryFrame cb;
    
    //cb.mWorld = XMMatrixTranspose(m_World);
    cb.vMeshColor = m_vMeshColor;
    
    m_CDevCont.g_pImmediateContextD3D11->UpdateSubresource(m_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    
    CBNeverChanges cbNeverChanges;

    if (m_SwitchCamera == -1) {

        cbNeverChanges.mView = m_QuieroDormir_Camara.GetView();
    }
    else if (m_SwitchCamera == 1) {

        cbNeverChanges.mView = m_QuieroDormir2_Camara.GetView();
    }

    m_CDevCont.g_pImmediateContextD3D11->UpdateSubresource(m_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);
    m_CDevCont.g_pImmediateContextD3D11->VSSetShader(m_CShader.g_pVertexShaderD3D11, NULL, 0);
    m_CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(0, 1, &m_pCBNeverChanges);
    m_CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(1, 1, &m_pCBChangeOnResize);
    m_CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame);
    m_CDevCont.g_pImmediateContextD3D11->PSSetShader(m_pPixelShader, NULL, 0);
    m_CDevCont.g_pImmediateContextD3D11->PSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame);
    m_CDevCont.g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &m_pTextureRV);
    m_CDevCont.g_pImmediateContextD3D11->PSSetSamplers(0, 1, &m_CSampleS.g_pSamplerLinearD3D11);
    m_CDevCont.g_pImmediateContextD3D11->DrawIndexed(36, 0, 0);

    for (int i = 0; i < m_Rows; i++) {

        for (int j = 0; j < m_Columns; j++) {

            if (m_LevelCubes[i][j]) {

                MapX += 2.5;
            }
            else if (m_LevelCubes[i][j] == m_Pilares) {

                MapX += 2.5;
            }
            else {

                MapX += 2.5;
            }
            
            if (m_LevelCubes[i][j] != 0) {

                m_CDevCont.g_pImmediateContextD3D11->VSSetShader(m_CShader.g_pVertexShaderD3D11, NULL, 0);
                m_CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(0, 1, &m_pCBNeverChanges);
                m_CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(1, 1, &m_pCBChangeOnResize);
                m_CDevCont.g_pImmediateContextD3D11->VSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame);
                m_CDevCont.g_pImmediateContextD3D11->PSSetShader(m_pPixelShader, NULL, 0);
                m_CDevCont.g_pImmediateContextD3D11->PSSetConstantBuffers(2, 1, &m_pCBChangesEveryFrame);
                m_CDevCont.g_pImmediateContextD3D11->PSSetShaderResources(0, 1, &m_pTextureRV);
                m_CDevCont.g_pImmediateContextD3D11->PSSetSamplers(0, 1, &m_CSampleS.g_pSamplerLinearD3D11);
                m_CDevCont.g_pImmediateContextD3D11->DrawIndexed(36, 0, 0);
            }

            m_World         = XMMatrixTranslation(MapX, 0, MapY);
            //cb.mWorld       = XMMatrixTranspose(m_World);
            cb.vMeshColor   = m_vMeshColor;

            m_CDevCont.g_pImmediateContextD3D11->UpdateSubresource(m_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
        }
        MapY += 2.5;
        MapX = 0;
    }
    
    m_CSwap.g_pSwapChainD3D11->Present(0, 0);
}

void ClaseManagerInterfaz::Destroy(){

    if (m_CDevCont.g_pImmediateContextD3D11) m_CDevCont.g_pImmediateContextD3D11->ClearState();

    if (m_CSampleS.g_pSamplerLinearD3D11) m_CSampleS.g_pSamplerLinearD3D11->Release();
    if (m_pTextureRV) m_pTextureRV->Release();
    if (m_pCBNeverChanges) m_pCBNeverChanges->Release();
    if (m_pCBChangeOnResize) m_pCBChangeOnResize->Release();
    if (m_pCBChangesEveryFrame) m_pCBChangesEveryFrame->Release();
    if (m_pVertexBuffer) m_pVertexBuffer->Release();
    if (m_pIndexBuffer) m_pIndexBuffer->Release();
    if (m_CShader.LayoutD3D11) m_CShader.LayoutD3D11->Release();
    if (m_CShader.g_pVertexShaderD3D11) m_CShader.g_pVertexShaderD3D11->Release();
    if (m_pPixelShader) m_pPixelShader->Release();
    if (m_CRendTar.g_pDepthStencilD3D11) m_CRendTar.g_pDepthStencilD3D11->Release();
    if (m_CDepthS.g_pDepthStencilViewD3D11) m_CDepthS.g_pDepthStencilViewD3D11->Release();
    if (m_CRendTarView.g_pRenderTargetViewD3D11) m_CRendTarView.g_pRenderTargetViewD3D11->Release();
    if (m_CSwap.g_pSwapChainD3D11) m_CSwap.g_pSwapChainD3D11->Release();
    if (m_CDevCont.g_pImmediateContextD3D11) m_CDevCont.g_pImmediateContextD3D11->Release();
    if (m_CDev.g_pd3dDeviceD3D11) m_CDev.g_pd3dDeviceD3D11->Release();
}

int __stdcall ClaseManagerInterfaz::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow){

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (FAILED(InitWindow(hInstance, nCmdShow))){

        return 0;
    }

    if (FAILED(Init())){

        Destroy();
        return 0;
    }

    Destroy();
    return (int)m_msg.wParam;
}

HRESULT ClaseManagerInterfaz::InitWindow(HINSTANCE hInstance, int nCmdShow){

    WNDCLASSEX wcex;
    wcex.cbSize             = sizeof(WNDCLASSEX);
    wcex.style              = CS_HREDRAW | CS_VREDRAW;
    //wcex.lpfnWndProc        = WndProc;
    wcex.cbClsExtra         = 0;
    wcex.cbWndExtra         = 0;
    wcex.hInstance          = hInstance;
    wcex.hIcon              = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor            = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground      = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName       = NULL;
    wcex.lpszClassName      = L"TutorialWindowClass";
    wcex.hIconSm            =  LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    
    if (!RegisterClassEx(&wcex)){

        return E_FAIL;
    }

    m_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindow(  L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, 
                            rc.bottom - rc.top, NULL, NULL, 
                            hInstance, NULL
                          );
    
    if (!m_hWnd){

        return E_FAIL;
    }

    ShowWindow(m_hWnd, nCmdShow);
    return S_OK;
}

HRESULT ClaseManagerInterfaz::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut){

    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob; //Un buffer que va a contener el resultado del shader.
    
    hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);

    if (FAILED(hr)){

        if (pErrorBlob != NULL){

            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        }
        if (pErrorBlob) pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

HRESULT ClaseManagerInterfaz::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout){

    ID3D11ShaderReflection* pVertexShaderReflection = NULL;
    if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection))){

        return S_FALSE;
    }

    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->GetDesc(&shaderDesc);

    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

    int offset = 0;

    for (int i = 0; i < shaderDesc.InputParameters; i++)
    {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName            = paramDesc.SemanticName;
        elementDesc.SemanticIndex           = paramDesc.SemanticIndex;
        elementDesc.InputSlot               = 0;
        elementDesc.AlignedByteOffset       = offset;
        elementDesc.InputSlotClass          = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate    = 0;

        if (paramDesc.Mask == 1){

            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        else if (paramDesc.Mask <= 3){

            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        else if (paramDesc.Mask <= 15){

            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; offset += 12;
        }
        else if (paramDesc.Mask <= 7){

            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        inputLayoutDesc.push_back(elementDesc);
    }

    HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

    pVertexShaderReflection->Release();
    return hr;
}

void ClaseManagerInterfaz::Resize(){

    if (m_CSwap.g_pSwapChainD3D11) {

        RECT rc;
        GetClientRect(m_hWnd, &rc);
        m_CDevCont.g_pImmediateContextD3D11->OMSetRenderTargets(0, 0, 0);

        m_CRendTarView.g_pRenderTargetViewD3D11->Release();
        m_CDepthS.g_pDepthStencilViewD3D11->Release();
        m_CRendTar.g_pDepthStencilD3D11->Release();

        HRESULT hr;
        hr = m_CSwap.g_pSwapChainD3D11->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

        ID3D11Texture2D* pBuffer;
        hr = m_CSwap.g_pSwapChainD3D11->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);

        hr = m_CDev.g_pd3dDeviceD3D11->CreateRenderTargetView(pBuffer, NULL, &m_CRendTarView.g_pRenderTargetViewD3D11);
        pBuffer->Release();

        DepthStencilDesc descDepth2;
        descDepth2.Width                    = rc.right - rc.left;
        descDepth2.Height                   = rc.bottom - rc.top;
        descDepth2.MipLevels                = 1;
        descDepth2.ArraySize                = 1;
        descDepth2.Format                   = FORMAT_D24_UNORM_S8_UINT;
        descDepth2.SampleDesc.My_Count      = 1;
        descDepth2.SampleDesc.My_Quality    = 0;
        descDepth2.Usage                    = USAGE_DEFAULT;
        descDepth2.BindFlags                = BIND_DEPTH_STENCIL;
        descDepth2.CPUAccessFlags           = 0;
        descDepth2.MiscFlags                = 0;
        m_CDepthS.Init(descDepth2);

        hr = m_CDev.g_pd3dDeviceD3D11->CreateTexture2D(&m_CDepthS.descDepthD3D11, NULL, &m_CRendTar.g_pDepthStencilD3D11);
        
        if (FAILED(hr)){

            return;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));

        descDSV.Format                  = m_CDepthS.descDepthD3D11.Format;
        descDSV.ViewDimension           = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice      = 0;
        hr = m_CDev.g_pd3dDeviceD3D11->CreateDepthStencilView(m_CRendTar.g_pDepthStencilD3D11, &descDSV, &m_CDepthS.g_pDepthStencilViewD3D11);
        
        if (FAILED(hr)){

            return;
        }

        m_CDevCont.g_pImmediateContextD3D11->OMSetRenderTargets(1, &m_CRendTarView.g_pRenderTargetViewD3D11, m_CDepthS.g_pDepthStencilViewD3D11);

        ViewportDesc vp2;
        UINT width      = rc.right - rc.left;
        UINT height     = rc.bottom - rc.top;
        vp2.Width       = width;
        vp2.Height      = height;
        vp2.MinDepth    = 0.0f;
        vp2.MaxDepth    = 1.0f;
        vp2.TopLeftX    = 0;
        vp2.TopLeftY    = 0;
        m_CView.Init(vp2);

        m_CDevCont.g_pImmediateContextD3D11->RSSetViewports(1, &m_CView.vpD3D11);

        m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

        CBChangeOnResize cbChangesOnResize;
        m_CDevCont.g_pImmediateContextD3D11->UpdateSubresource(m_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

        m_QuieroDormir_Camara.SetHeight(height);
        m_QuieroDormir_Camara.SetWidht(width);
        m_QuieroDormir_Camara.GenerateProjectionMatrix();

        m_QuieroDormir2_Camara.SetHeight(height);
        m_QuieroDormir2_Camara.SetWidht(width);
        m_QuieroDormir2_Camara.GenerateProjectionMatrix();

        cbChangesOnResize.mProjection = m_QuieroDormir2_Camara.GetProjection();
        m_CDevCont.g_pImmediateContextD3D11->UpdateSubresource(m_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
    }
}

void ClaseManagerInterfaz::InitCameras(){

    CameraDescriptor GodCamera;
    GodCamera.s_At = { 0,0,0 };
    GodCamera.s_Eye = { 0,0,1 };
    GodCamera.s_Up = { 0,1,0 };
    GodCamera.s_Far = 1000;
    GodCamera.s_Near = 0.01;
    GodCamera.s_FoV = XM_PIDIV4;
    GodCamera.s_Height = m_height;
    GodCamera.s_Widht = m_width;

    CameraDescriptor FirstCamera;
    FirstCamera.s_At = { 0,0,0 };
    FirstCamera.s_Eye = { 0,0,-6 };
    FirstCamera.s_Up = { 0,1,0 };
    FirstCamera.s_Far = 1000;
    FirstCamera.s_Near = 0.01;
    FirstCamera.s_FoV = XM_PIDIV4;
    FirstCamera.s_Height = m_height;
    FirstCamera.s_Widht = m_width;

    m_QuieroDormir_Camara.Init(GodCamera);
    m_QuieroDormir2_Camara.Init(FirstCamera);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

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

        case WM_SIZE:
            //Resize();
            break;

        case WM_KEYDOWN:
            //if (m_SwitchCamera == -1) {
            //
            //    m_QuieroDormir_Camara.inputs(wParam);
            //}
            //else if (m_SwitchCamera == 1) {
            //
            //    m_QuieroDormir2_Camara.inputs(wParam);
            //}
            break;

        case WM_LBUTTONDOWN:
            //if (m_SwitchCamera == -1) {//In this case We can move the free camera
            //
            //    GetCursorPos(&Temp);
            //    m_QuieroDormir_Camara.SetOriginalMousePos(Temp.x, Temp.y);
            //    m_ClickPressed = true;
            //}
            //else if (m_SwitchCamera == 1) {//In this case We can move the first camera
            //
            //    GetCursorPos(&Temp);
            //    m_QuieroDormir2_Camara.SetOriginalMousePos(Temp.x, Temp.y);
            //    m_ClickPressed = true;
            //}
            break;

        case WM_LBUTTONUP:
            //m_ClickPressed = false;
            break;

        case WM_RBUTTONDOWN:
            //m_SwitchCamera *= -1;
            break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}*/