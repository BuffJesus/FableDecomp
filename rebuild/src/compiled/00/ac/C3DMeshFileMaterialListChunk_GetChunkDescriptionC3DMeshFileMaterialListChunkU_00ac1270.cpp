// Faithful VC7.1 translation of C3DMeshFileMaterialListChunk_Methods::GetChunkDescription (00ac1270)
// Struct-return const method: return CCharString("...", -1);

#include "engine/C3DMeshFileMaterialListChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    char* m_ptr;
    int   m_len;
    // ctor at 0x0099ebf0: CCharString(const char* str, long len)
    CCharString(const char* str, long len);
};

// The literal at 0x129ee0c
static const char* const s_desc = (const char*)0x129ee0c;

struct C3DMeshFileMaterialListChunk_Methods : C3DMeshFileMaterialListChunk {
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileMaterialListChunk_Methods::GetChunkDescription() const {
    return CCharString(s_desc, -1);
}