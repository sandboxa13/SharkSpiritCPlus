#include "GraphicsManager.h"

#pragma comment (lib, "D3D11.lib")
#pragma comment (lib, "d3dcompiler.lib")

GraphicsManager::GraphicsManager(HWND hwnd)
{
    Initialize(hwnd);
}

HRESULT GraphicsManager::Initialize(HWND &hwnd)
{
    HRESULT hr = S_OK;

    RECT rect;

    GetClientRect(hwnd, &rect);

    UINT width = rect.right - rect.left;
    UINT height = rect.bottom - rect.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        auto driver_type = driverTypes[driverTypeIndex];

        hr = D3D11CreateDevice(nullptr, driver_type, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
            D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_immediateContext);

        if (hr == E_INVALIDARG)
        {
            // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
            hr = D3D11CreateDevice(nullptr, driver_type, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_immediateContext);
        }

        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = m_device.Get()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }

    if (FAILED(hr))
        return hr;


    IDXGIFactory2* dxgiFactory2 = nullptr;
    hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
    if (dxgiFactory2)
    {
        // DirectX 11.1 or later
        hr = m_device.Get()->QueryInterface(__uuidof(ID3D11Device1), (&m_device1));
        if (SUCCEEDED(hr))
        {
            (void)m_immediateContext.Get()->QueryInterface(__uuidof(ID3D11DeviceContext1), (&m_immediateContext1));
        }

        DXGI_SWAP_CHAIN_DESC1 sd = {};
        sd.Width = width;
        sd.Height = height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;

        hr = dxgiFactory2->CreateSwapChainForHwnd(m_device.Get(), hwnd, &sd, nullptr, nullptr, &m_pSwapChain1);
        if (SUCCEEDED(hr))
        {
            hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), (&m_pSwapChain));
        }

        dxgiFactory2->Release();
    }
    else
    {
        // DirectX 11.0 systems
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Width = width;
        sd.BufferDesc.Height = height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;

        hr = dxgiFactory->CreateSwapChain(m_device.Get(), &sd, &(m_pSwapChain));
    }

    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain.Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
    if (FAILED(hr))
        return hr;

    D3D11_RENDER_TARGET_VIEW_DESC render_targer_desc;
    render_targer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    render_targer_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    hr = m_device.Get()->CreateRenderTargetView(pBackBuffer, nullptr, m_pRenderTargetView.GetAddressOf());
    pBackBuffer->Release();
    if (FAILED(hr))
        return hr;

    m_immediateContext.Get()->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), nullptr);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_immediateContext.Get()->RSSetViewports(1, &vp);

    ID3DBlob* pBlob = nullptr;

    // load and set vertex shader
    {
        hr = LoadShader(L"VertexShader.cso", &pBlob);

        if (FAILED(hr))
            return hr;

        hr = m_device.Get()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader);

        if (FAILED(hr))
        {
            pBlob->Release();
            return hr;
        }

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        hr = m_device.Get()->CreateInputLayout(layout, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pVertexLayout);

        if (FAILED(hr))
            return hr;

        m_immediateContext.Get()->IASetInputLayout(m_pVertexLayout.Get());
    }
    
    // load and set pixel shader
    {
        hr = LoadShader(L"PixelShader.cso", &pBlob);

        if (FAILED(hr))
            return hr;

        hr = m_device.Get()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);

        if (FAILED(hr))
        {
            pBlob->Release();
            return hr;
        }
    }

    struct Vertex
    {
        DirectX::XMFLOAT3 Pos;
    };

    Vertex vertices[] =
    {
        DirectX::XMFLOAT3(0.0f, 0.5f, 0.5f),
        DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),
        DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),
    };
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;
    hr = m_device.Get()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);

    if (FAILED(hr))
        return hr;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_immediateContext.Get()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

    // Set primitive topology
    m_immediateContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return S_OK;
}

HRESULT GraphicsManager::LoadShader(const WCHAR* szFileName, ID3DBlob** ppBlobOut)
{
    return D3DReadFileToBlob(szFileName, ppBlobOut);
}


void GraphicsManager::Render()
{
    m_immediateContext.Get()->ClearRenderTargetView(m_pRenderTargetView.Get(), DirectX::Colors::DarkBlue);

    m_immediateContext.Get()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
    m_immediateContext.Get()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

    m_immediateContext.Get()->Draw(3, 0);

    m_pSwapChain->Present(0, 0);
}
