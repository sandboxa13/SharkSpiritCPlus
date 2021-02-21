#pragma once

#include <d3d11_1.h>
#include <wrl.h>
#include <directxcolors.h>

using namespace Microsoft::WRL;

class SSDevice
{
public:
	SSDevice(HWND hwnd);
	~SSDevice() = default;
	void Render();

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11Device1> m_device1;
	ComPtr<ID3D11DeviceContext> m_immediateContext;
	ComPtr<ID3D11DeviceContext1> m_immediateContext1;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<IDXGISwapChain1> m_pSwapChain1;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

private:
	HRESULT InitializeDeviceAndSwapChain(HWND &hwnd);
};