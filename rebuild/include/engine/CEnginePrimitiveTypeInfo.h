#ifndef FABLE_ENGINE_CENGINEPRIMITIVETYPEINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVETYPEINFO_H
// Original component/noncopyable bases and47 manager pointers, donor/retail
// verified by CEngineScene::UpdatePosition at00B91840.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_engine_component.h"
#include "fable_base_class.h"

class CEnginePrimitiveManager;

#pragma pack(push, 1)
class CEnginePrimitiveTypeInfo : public CEngineComponent, public CBaseClassNonCopyable {
public:
    virtual unsigned long PeekSceneFilterFlags() const;
    CEnginePrimitiveManager* PrimitiveManagers[47];          // +0x10
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveTypeInfo) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveTypeInfo, PrimitiveManagers) == 0x10);

#endif // FABLE_ENGINE_CENGINEPRIMITIVETYPEINFO_H
