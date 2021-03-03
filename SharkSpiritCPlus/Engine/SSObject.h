#pragma once

#include <rpc.h>
#include "..//Render//RenderObject.h"

class SSObject 
{
public:
	SSObject();
private:
	UUID m_id;
	RenderObject* m_renderObject;
};