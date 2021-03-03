#include "RenderPipeline.h"

RenderPipeline::RenderPipeline()
{
}

void RenderPipeline::Bind()
{
    for (auto& stage : m_stages)
    {
        //todo bind stage
    }
}

void RenderPipeline::AddStage(RenderStageBase& stage)
{
    m_stages.push_back(stage);
}
