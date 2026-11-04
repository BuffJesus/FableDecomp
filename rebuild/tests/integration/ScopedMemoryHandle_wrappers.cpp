// Fixture entry points only. Ownership behavior comes from the canonical header.
// These synthetic entry addresses never count as reconstructed retail functions.
#include "fable_handle_memory_pool.h"
#include <new>
extern "C" CScopedMemoryHandle* __fastcall ConstructScopedHandle(void* storage)
{
    return new (storage) CScopedMemoryHandle;
}
extern "C" void __fastcall ClearScopedHandle(CScopedMemoryHandle* owner)
{
    owner->Clear();
}
extern "C" void __fastcall DestroyScopedHandle(CScopedMemoryHandle* owner)
{
    owner->~CScopedMemoryHandle();
}
