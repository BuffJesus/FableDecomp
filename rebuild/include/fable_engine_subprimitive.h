#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_base_class.h"
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
// Ego_r original base and fields; retail RenderType reads Manager at +0x14.
class CEngineSubPrimitiveBase : public CBaseClassNonCopyable
{
public:
    virtual ~CEngineSubPrimitiveBase();
    unsigned long RenderFlags;
    unsigned char Alpha;
    CEngineInternalPrimitiveBase* Parent;
    CEngineSubPrimitiveBase* NextSubPrimitive;
    CEnginePrimitiveManager* Manager;
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveBase)==24);
FABLE_STATIC_ASSERT(offsetof(CEngineSubPrimitiveBase,Manager)==20);
