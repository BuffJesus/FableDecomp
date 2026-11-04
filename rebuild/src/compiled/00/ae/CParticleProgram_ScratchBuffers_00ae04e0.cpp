// CParticleProgram's private static scratch buffers (Ego_r names; retail GetMatrixBuffer 0x00AE04E0, GetVectorBuffer
// 0x00AE0540, GetGroupVectorBuffer 0x00AE05A0). Each buffer only grows: a larger request frees its pool and carves a
// fresh block of count elements (none when count is 0 or the byte size overflows 32 bits); a request that fits
// returns the current block. Retail constructs each pool as CSimpleMemoryPool(0, 0x1000) (dynamic initializers
// 0x01224450 / 0x01224470 / 0x01224490).
#include "fable_particle_program.h"
#include "fable_matrix4x4.h"
#include "fable_simple_memory_pool.h"

CMatrix4x4* CParticleProgram::MatrixBuffer = 0;
unsigned long CParticleProgram::MatrixBufferSize = 0;
C4DVector* CParticleProgram::VectorBuffer = 0;
unsigned long CParticleProgram::VectorBufferSize = 0;
C4DVector* CParticleProgram::GroupVectorBuffer = 0;
unsigned long CParticleProgram::GroupVectorBufferSize = 0;

static CSimpleMemoryPool MatrixBufferPool(0, 0x1000);
static CSimpleMemoryPool VectorBufferPool(0, 0x1000);
static CSimpleMemoryPool GroupVectorBufferPool(0, 0x1000);

// Shared by the three getters: no block for an empty request or one whose byte size overflows 32 bits.
template <class T> static inline T* AllocateBuffer(CSimpleMemoryPool& pool, unsigned long count)
{
    if (count)
    {
        if ((unsigned __int64)count * sizeof(T) > 0xFFFFFFFFu)
            return 0;
        return static_cast<T*>(pool.Allocate(count * sizeof(T)));
    }
    return 0;
}

CMatrix4x4* FABLE_FASTCALL CParticleProgram::GetMatrixBuffer(unsigned long count)
{
    if (count > MatrixBufferSize)
    {
        MatrixBufferPool.FreeAll();
        CMatrix4x4* buffer = AllocateBuffer<CMatrix4x4>(MatrixBufferPool, count);
        MatrixBufferSize = count;
        MatrixBuffer = buffer;
    }
    return MatrixBuffer;
}

C4DVector* FABLE_FASTCALL CParticleProgram::GetVectorBuffer(unsigned long count)
{
    if (count > VectorBufferSize)
    {
        VectorBufferPool.FreeAll();
        C4DVector* buffer = AllocateBuffer<C4DVector>(VectorBufferPool, count);
        VectorBufferSize = count;
        VectorBuffer = buffer;
    }
    return VectorBuffer;
}

C4DVector* FABLE_FASTCALL CParticleProgram::GetGroupVectorBuffer(unsigned long count)
{
    if (count > GroupVectorBufferSize)
    {
        GroupVectorBufferPool.FreeAll();
        C4DVector* buffer = AllocateBuffer<C4DVector>(GroupVectorBufferPool, count);
        GroupVectorBufferSize = count;
        GroupVectorBuffer = buffer;
    }
    return GroupVectorBuffer;
}
