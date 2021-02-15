#pragma once

#include <string>

class RenderStageBase
{
public:
	std::string name;
	bool IsIndexBuffer;
	RenderStageBase();

protected:
	virtual void BindToPipeLine();
};