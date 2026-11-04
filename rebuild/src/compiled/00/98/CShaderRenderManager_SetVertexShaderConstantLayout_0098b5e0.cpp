#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
namespace NVectorMath {
void __cdecl GFClear4DVectors(C4DVector* vectors,unsigned long count,const C4DVector& value);
}
static __forceinline bool DifferentRange(const CShaderConstantRange& a,const CShaderConstantRange& b)
{
    return a.Offset!=b.Offset || a.Count!=b.Count;
}
void CShaderRenderManager::SetVertexShaderConstantLayout(long index)
{
    if(index==-1)
    {
        CurrentVSConstantLayoutIndex=-1;
        CurrentVSConstantLayout=&NullConstantLayout;
        return;
    }
    const CVertexShaderConstantLayout* next=ConstantLayouts()[index];
    if(CurrentVSConstantLayout)
    {
        updateFlags3D8 |= 4;
        if(DifferentRange(CurrentVSConstantLayout->WorldMatrix,next->WorldMatrix))updateFlags3D8 |= 0x10;
        if(DifferentRange(CurrentVSConstantLayout->FogTransform,next->FogTransform))updateFlags3D8 |= 0x6000;
        if(DifferentRange(CurrentVSConstantLayout->Ambient,next->Ambient))updateFlags3D8 |= 4;
        if(DifferentRange(CurrentVSConstantLayout->FogColour,next->FogColour))updateFlags3D8 |= 0x20000;
        if(DifferentRange(CurrentVSConstantLayout->CameraPos,next->CameraPos))updateFlags3D8 |= 8;
        const unsigned long lightSize=next->LightSize;
        if(CurrentVSConstantLayout->LightSize!=lightSize ||
           DifferentRange(CurrentVSConstantLayout->LightArray,next->LightArray) ||
           CurrentVSConstantLayout->LightGlobals!=next->LightGlobals)
        {
            if(static_cast<long>(lightSize)>0)
            {
                {
                    CShaderConstantRange range=next->LightArray;
                    LightCount=range.Count/lightSize;
                }
                if(LightCount>AllocatedLights || lightSize>AllocatedLightSize)
                    AllocateLights(LightCount,lightSize);
                for(unsigned long light=0;light<LightCount;++light)
                    Lights[light].UpdateFlag=true;
                C4DVector value;
                value.X=0.0f;value.Y=0.0f;value.Z=0.0f;value.W=1.0f;
                NVectorMath::GFClear4DVectors(LightBuffer,LightCount*lightSize,value);
            }
            updateFlags3D8 |= 0x700;
        }
        if(DifferentRange(CurrentVSConstantLayout->LightGlobals,next->LightGlobals))
        {
            unsigned long count=next->LightGlobals.Count;
            if(count<=LightGlobalVector().size())count=LightGlobalVector().size();
            C4DVector value;
            value.X=0.0f;value.Y=0.0f;value.Z=0.0f;value.W=1.0f;
            LightGlobalVector().resize(count,value);
            updateFlags3D8 |= 0x400;
        }
        if(DifferentRange(CurrentVSConstantLayout->CombinedProjectionMatrix,next->CombinedProjectionMatrix))
            updateFlags3D8 |= 0x80;
    }
    CurrentVSConstantLayout=next;
    CurrentVSConstantLayoutIndex=index;
}
