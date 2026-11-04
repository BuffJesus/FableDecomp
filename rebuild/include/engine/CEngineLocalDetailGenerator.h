#pragma once
#include <stddef.h>
#include "fable_engine_component.h"
#include "fable_carray.h"
class CEngineMap;
class CEngineVideoOptionsDef;
class CLocalDetailObjectCollectionType;
struct CRuntimeFadeDistanceModiferTableEntry {
    float DistanceClampMin;
    float DistanceFactor;
};
// Partial retail view; donor base and B59C70 establish the array at0xc.
// No allocation size is asserted.
class CEngineLocalDetailGenerator : public CEngineComponent {
public:
    void UpdateLoadingDistanceVideoOptions(const CEngineVideoOptionsDef&);
    void CreateMap(CEngineMap*);
    void DeleteMap(CEngineMap*);
    const CLocalDetailObjectCollectionType* RegisterObjectCollectionType(const CLocalDetailObjectCollectionType&);
    CArrayVector<CLocalDetailObjectCollectionType*> ObjectCollectionTypes;
    unsigned char Unrecovered18[0x3c-0x18];
    CRuntimeFadeDistanceModiferTableEntry RuntimeFadeDistanceModiferTable[8];
};
FABLE_STATIC_ASSERT(offsetof(CEngineLocalDetailGenerator,ObjectCollectionTypes)==0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineLocalDetailGenerator,RuntimeFadeDistanceModiferTable)==0x3c);
