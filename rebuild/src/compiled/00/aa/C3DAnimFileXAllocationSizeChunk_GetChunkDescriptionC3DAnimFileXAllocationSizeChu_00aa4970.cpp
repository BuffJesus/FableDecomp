#include "engine/C3DAnimFileXAllocationSizeChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString { char* Data; CCharString(const char* text, int length); };
struct C3DAnimFileXAllocationSizeChunk_Methods : C3DAnimFileXAllocationSizeChunk {
    CCharString GetChunkDescription() const;
};
CCharString C3DAnimFileXAllocationSizeChunk_Methods::GetChunkDescription() const
{
    return CCharString("AllocationSize", -1);
}
