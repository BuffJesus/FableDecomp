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

struct CPixelFormatByteLengthView
{
    fable_u32 value00;

    fable_u32 GetColourDepth() const;
    void Initialise(fable_u32 format);
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

struct CTexturePreallocatedView
{
    FableLifecycleTexture* texture00;
    union
    {
        fable_u32 flags04;
        struct
        {
            fable_u32 ByteLength : 28;
            fable_u32 AllocationSource : 4;
        };
    };

    void CalcByteLength();
    bool InitialiseFromPreallocatedTexture(
        FableLifecycleTexture* texture);
};

FABLE_STATIC_ASSERT(sizeof(CTextureAssignmentView) == 8);
FABLE_STATIC_ASSERT(sizeof(CTextureUninitialiseView) == 8);
FABLE_STATIC_ASSERT(sizeof(CTexturePreallocatedView) == 8);
