#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEnginePrimitiveRenderer.h"

void CEngine::AddPrimitive(const CEnginePrimitive& primitive)
{ NEngineInternal::GPPrimitiveRenderer->AddPrimitive(primitive); }
void CEngine::SetPersistentPrimitive(CEnginePrimitiveHandle& handle,const CEnginePrimitive& primitive,unsigned long flags)
{ NEngineInternal::GPPrimitiveRenderer->SetPersistentPrimitive(handle,primitive,flags,false); }
unsigned long CEngine::AddChildPrimitive(CEnginePrimitiveHandle& handle,const CEnginePrimitive& primitive,unsigned long flags)
{ return NEngineInternal::GPPrimitiveRenderer->AddChildPrimitive(handle,primitive,flags); }
void CEngine::SetChildPrimitive(CEnginePrimitiveHandle& handle,unsigned long index,CEnginePrimitiveHandle& child,const CEnginePrimitive& primitive,unsigned long flags)
{ NEngineInternal::GPPrimitiveRenderer->SetChildPrimitive(handle,index,child,primitive,flags); }
void CEngine::SetChildPrimitive(CEnginePrimitiveHandle& handle,unsigned long index,const CEnginePrimitive& primitive,unsigned long flags)
{ NEngineInternal::GPPrimitiveRenderer->SetChildPrimitive(handle,index,primitive,flags); }
void CEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle& handle,unsigned long index,CEnginePrimitiveHandle& child)
{ NEngineInternal::GPPrimitiveRenderer->GetChildPrimitiveHandle(handle,index,child); }
void CEngine::ClearChildPrimitive(CEnginePrimitiveHandle& handle,unsigned long index)
{ NEngineInternal::GPPrimitiveRenderer->ClearChildPrimitive(handle,index); }
void CEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle& handle)
{ NEngineInternal::GPPrimitiveRenderer->ClearAllChildPrimitives(handle); }
