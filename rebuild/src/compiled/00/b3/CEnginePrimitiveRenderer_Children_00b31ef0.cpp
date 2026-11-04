#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineInternalPrimitiveBase.h"
unsigned long CEnginePrimitiveRenderer::AddChildPrimitive(CEnginePrimitiveHandle& handle,const CEnginePrimitive& value,unsigned long flags)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(!primitive)return 0;
    return primitive->AddChildPrimitive(value,flags);
}
void CEnginePrimitiveRenderer::SetChildPrimitive(CEnginePrimitiveHandle& handle,unsigned long index,CEnginePrimitiveHandle& child,const CEnginePrimitive& value,unsigned long flags)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive)primitive->SetChildPrimitive(index,child,value,flags);
}
void CEnginePrimitiveRenderer::GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle,unsigned long index,CEnginePrimitiveHandle& child)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive)primitive->GetChildPrimitiveHandle(index,child);
}
void CEnginePrimitiveRenderer::SetChildPrimitive(CEnginePrimitiveHandle& handle,unsigned long index,const CEnginePrimitive& value,unsigned long flags)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive)primitive->SetChildPrimitive(index,value,flags);
}
void CEnginePrimitiveRenderer::ClearChildPrimitive(CEnginePrimitiveHandle& handle,unsigned long index)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive)primitive->ClearChildPrimitive(index);
}
void CEnginePrimitiveRenderer::ClearAllChildPrimitives(CEnginePrimitiveHandle& handle)
{
    CEngineInternalPrimitiveBase* primitive=static_cast<CEngineInternalPrimitiveBase*>(handle.Data.data_);
    if(primitive)primitive->ClearAllChildPrimitives();
}
