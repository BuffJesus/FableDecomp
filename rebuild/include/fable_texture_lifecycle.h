#pragma once

#include "rebuild_abi.h"

struct FableLifecycleTexture;

typedef fable_u32 (__stdcall *FableLifecycleTextureCall)(
    FableLifecycleTexture* texture);

struct FableLifecycleTextureVTable
{
    void* queryInterface00;
    FableLifecycleTextureCall addRef04;
    FableLifecycleTextureCall release08;
};

struct FableLifecycleTexture
{
    FableLifecycleTextureVTable* vtable;
};

struct CTextureAssignmentView
{
    FableLifecycleTexture* PD3DTexture;
    fable_u32 ByteLength : 28;
    fable_u32 AllocationSource : 4;

    CTextureAssignmentView& operator=(CTextureAssignmentView& rhs);
};

struct CTextureUninitialiseView
{
    FableLifecycleTexture* texture00;
    fable_u32 flags04;

    void Uninitialise();
};

FABLE_STATIC_ASSERT(sizeof(CTextureAssignmentView) == 8);
FABLE_STATIC_ASSERT(sizeof(CTextureUninitialiseView) == 8);
