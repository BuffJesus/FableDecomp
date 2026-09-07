#include "engine/CChunkedFileRawDataChunk.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
// CChunkedFileRawDataChunk_Methods::IsUnknownType @ 0x00aa7740
// mov eax,1; ret  -> always true
struct CChunkedFileRawDataChunk_Methods : CChunkedFileRawDataChunk {
    unsigned long IsUnknownType();
};
unsigned long CChunkedFileRawDataChunk_Methods::IsUnknownType(){ return 1; }