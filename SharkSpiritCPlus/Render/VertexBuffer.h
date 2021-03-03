#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "GraphicsManager.h"

using namespace Microsoft::WRL;

struct Vertex
{
	DirectX::XMFLOAT3 Pos;
};

class VertexBuffer
{
public:
	VertexBuffer(GraphicsManager& gManager, Vertex vertices[]);
	~VertexBuffer();

private:
	ComPtr<ID3D11Buffer> m_vertexBuffer;
};