#pragma once

#include "RenderStage.h"
#include <vector>


class RenderPipeline 
{
private:
	std::vector<RenderStageBase> m_stages;
public:
	RenderPipeline();
	void Bind();
	void AddStage(RenderStageBase& stage);
};