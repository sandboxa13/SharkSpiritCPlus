#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(GraphicsManager& gManager, Vertex vertices[])
{
	HRESULT hr = S_OK;

	ID3DBlob* pBlob = nullptr;

	hr = gManager.LoadShader(L"VertexShader.cso", &pBlob);

	if (FAILED(hr))
		return;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(Vertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;

    hr = gManager.GetDevice().Get()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    gManager.GetDeviceContext().Get()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    pBlob->Release();
}

VertexBuffer::~VertexBuffer()
{
}
