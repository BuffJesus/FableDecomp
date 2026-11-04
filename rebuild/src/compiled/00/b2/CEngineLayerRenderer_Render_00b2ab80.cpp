#include "fable_engine_layer_renderer.h"
#include "fable_engine_component_params.h"
void CEngineLayerRenderer::Render(const CEngineComponentParams& params)
{
    if(!(SceneFilterFlags&params.SceneFilterFlags))return;
    CEngineComponentRenderParams render={&params,Layer,Flags};
    for(unsigned i=0;i<Components.size();++i)
        if(Components[i]->PeekSceneFilterFlags()&params.SceneFilterFlags)
            Components[i]->PreLayerRender(render);
    for(unsigned j=0;j<Components.size();++j)
        if((Components[j]->PeekSceneFilterFlags()&params.SceneFilterFlags)&&Components[j]->RenderEnabled)
            Components[j]->Render(render);
    for(unsigned k=0;k<Components.size();++k)
        if(Components[k]->PeekSceneFilterFlags()&params.SceneFilterFlags)
            Components[k]->PostLayerRender(render);
}
