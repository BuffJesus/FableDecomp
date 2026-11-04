#include "engine/CEngineSubPrimitiveRenderer.h"
#include "fable_engine_subprimitive.h"
#include "fable_engine_primitive_manager.h"
#include "fable_engine_internal.h"
#include "fable_display_viewport.h"
void CEngineSubPrimitiveRenderer::RenderType(ESubPrimitiveType type,bool flush)
{
    FableCurrentRenderStateManager_013cb508->StartCaptureBlock();
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    CSubPrimitiveList& list=SubPrimitiveLists[type];
    if(list.Enable){
        if(list.Sorted)SortList(list.Elements,list.RequiresStableSort);
        if(list.BoundingVolumePass){
            unsigned long count=list.Elements.size();
            for(unsigned long i=0;i<count;++i){
                CEngineSubPrimitiveBase* primitive=list.Elements[i].SubPrimitive;
                primitive->Manager->RenderBoundingVolume(*primitive);
            }
        }else if(list.PreRenderPass)RenderListTwoPass(list.Elements,list.PreRenderPass,list.RenderPass);
        else RenderList(list.Elements,list.RenderPass);
    }
    if(flush)DeleteList(list.Elements);
    FableCurrentRenderStateManager_013cb508->RestoreCaptureBlock();
}
