#pragma once

#include <d3d11_1.h>
#include <wrl.h>
#include <directxcolors.h>
#include <string>
#include <d3dcompiler.h>
#include <directxmath.h>

using namespace Microsoft::WRL;

class GraphicsManager
{
public:
	GraphicsManager(HWND hwnd);
	~GraphicsManager() = default;
	void Render();

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11Device1> m_device1;
	ComPtr<ID3D11DeviceContext> m_immediateContext;
	ComPtr<ID3D11DeviceContext1> m_immediateContext1;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<IDXGISwapChain1> m_pSwapChain1;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11VertexShader> m_pVertexShader;
	ComPtr<ID3D11PixelShader> m_pPixelShader;
	ComPtr<ID3D11InputLayout> m_pVertexLayout;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

private:
	HRESULT Initialize(HWND &hwnd);
	HRESULT LoadShader(const WCHAR* szFileName, ID3DBlob** ppBlobOut);
};