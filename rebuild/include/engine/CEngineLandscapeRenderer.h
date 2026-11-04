#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
class CEngineMap;
class CEngineLandscapeMemoryManager;
// Retail prefix used by native resource deletion at00BDFA10 and00B42941.
// Donor MemoryManager is atF58; retail is6B0. No full allocation size is claimed.
class CEngineLandscapeRenderer {
public:
    void CreateMap(CEngineMap*,long);
    void DeleteMap(CEngineMap*);
    unsigned char UnknownPrefix[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;
};
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer,MemoryManager)==0x6b0);
