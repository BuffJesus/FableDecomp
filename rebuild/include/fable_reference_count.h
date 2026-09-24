#pragma once
#include "rebuild_abi.h"

typedef void (FABLE_FASTCALL *FableDestroyReferencedObject)(void* object);
struct FableReferenceCount
{
    fable_i32 owners;
    FableDestroyReferencedObject destroy;
    void* object;
};
