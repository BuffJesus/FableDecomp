#ifndef FABLETLC_OUTLINE_GLOW_COMPILED_H
#define FABLETLC_OUTLINE_GLOW_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

extern fable_u8* DAT_01436ea8;

class LandscapeLayerList
{
public:
    void InsertNode(int node);
};

class CEngineLandscapeMemoryManager
{
public:
    void FreeVirtual(void* allocation);
};

class CEngineScreenEffectOutlineGlow
{
public:
    void** vfptr;
    fable_u8 m_Pad04[0x0C];
    int field_10;
    void* Blur2Texture;

    ~CEngineScreenEffectOutlineGlow();
    CEngineScreenEffectOutlineGlow* ScalarDeletingDestructor(unsigned int freeFlags);
};

FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, field_10) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectOutlineGlow, Blur2Texture) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CEngineScreenEffectOutlineGlow) == 0x18);

#endif
