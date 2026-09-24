#pragma once
#include "fable_ui_bank_runtime.h"
struct FableUiPreallocTexturePool
{
    CResourceList Resources;
    unsigned Count,MaxSize;
    int PoolIndex;
    FableUiGraphicArray Textures;
};
// The donor CTextureManager uses 0x58-byte pools; retail uses 0x54-byte pools
// because its vector has three words. Do not allocate using the donor's 0x614.
struct FableUiTextureManagerView
{
    FableUiGraphicsBankVtable* Vtable;
    bool Initialised;
    unsigned char Unrecovered05[3];
    unsigned PoolCount;
    unsigned PreallocSizes[16];
    FableUiPreallocTexturePool PreallocPools[16];
    unsigned FailedAllocations[16];
    int BufferSize;
    void* TextureBuffer;
};
FABLE_STATIC_ASSERT(sizeof(FableUiPreallocTexturePool)==0x54);
FABLE_STATIC_ASSERT(sizeof(FableUiTextureManagerView)==0x5D4);
FABLE_STATIC_ASSERT(offsetof(FableUiTextureManagerView,PreallocPools)==0x4C);
FABLE_STATIC_ASSERT(offsetof(FableUiTextureManagerView,FailedAllocations)==0x58C);
extern FableUiGraphicsBankVtable FableUiTextureManagerVtable; // 0129DC5C
extern void* FableUiPreallocPoolVtable; // 0129DC50
CResourceList* __fastcall FableUiConstructResourceList(CResourceList*,void*); // 009FC570
