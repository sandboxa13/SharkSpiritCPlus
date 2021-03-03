#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class RenderObject
{
public:
	RenderObject();
	~RenderObject();

private:
	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;
	ComPtr<ID3D11VertexShader> m_vertexShader;
};