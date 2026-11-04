#pragma once
// Ego_r CTemporaryBuffer (lib_temporary_buffer.hpp): a working buffer taken
// from the global scratch buffer when it is free and big enough, else from a
// handle-based pool (locked for the buffer's lifetime), else from the heap.
// The bodies are inline in the header; retail keeps them out of line as
// per-unit COMDATs (e.g. ctor 0x00A98860 / dtor 0x00A98910 next to
// C3DAnimFileLoader::LoadAnimFileCompressed). Vtable 0x0129DFD8.
#include <new>
#include "fable_base_class.h"
#include "fable_handle_memory_pool.h"
#include "fable_system_manager.h"

class CTemporaryBuffer : public CBaseClassNonCopyable
{
public:
    enum EAllocateType
    {
        ALLOCATED_FROM_MEMORY_POOL = 0,
        ALLOCATED_FROM_SCRATCH_BUFFER = 1,
        ALLOCATED_FROM_NEW = 2
    };

    CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* memory_pool, CScratchBuffer* scratch_buffer)
        : Data(0), MemoryPool(memory_pool), ScratchBuffer(scratch_buffer)
    {
        if (ScratchBuffer && size <= (unsigned long)ScratchBuffer->GetByteSize() && !ScratchBuffer->IsLocked())
        {
            ScratchBuffer->Lock(&Data);
            AllocateType = ALLOCATED_FROM_SCRATCH_BUFFER;
        }
        else if (MemoryPool && MemoryPool->Allocate(&MemoryHandle, size, 0))
        {
            MemoryPool->Lock(MemoryHandle);
            Data = MemoryPool->GetPointerFromHandle(MemoryHandle);
            AllocateType = ALLOCATED_FROM_MEMORY_POOL;
        }
        else
        {
            Data = new unsigned char[size];
            AllocateType = ALLOCATED_FROM_NEW;
        }
    }

    virtual ~CTemporaryBuffer()
    {
        switch (AllocateType)
        {
        case ALLOCATED_FROM_MEMORY_POOL:
            MemoryPool->Unlock(MemoryHandle);
            MemoryPool->Free(MemoryHandle);
            break;
        case ALLOCATED_FROM_SCRATCH_BUFFER:
            ScratchBuffer->Unlock();
            break;
        case ALLOCATED_FROM_NEW:
            delete[] static_cast<unsigned char*>(Data);
            break;
        }
    }

    // FableWin ?GetBuffer@CTemporaryBuffer@@QAEPAXXZ; inline in retail.
    void* GetBuffer() { return Data; }

    void* Data;                            // +0x04
    CHandleBasedMemoryPool* MemoryPool;    // +0x08
    long MemoryHandle;                     // +0x0C
    CScratchBuffer* ScratchBuffer;         // +0x10
    EAllocateType AllocateType;            // +0x14
};

FABLE_STATIC_ASSERT(sizeof(CTemporaryBuffer) == 0x18);
