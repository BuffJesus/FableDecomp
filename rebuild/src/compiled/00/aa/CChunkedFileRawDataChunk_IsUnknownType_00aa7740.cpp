#include "rebuild_abi.h"
// CChunkedFileRawDataChunk::IsUnknownType @ 0x00aa7740
// mov eax,1; ret  -> always true
struct CChunkedFileRawDataChunk { unsigned long IsUnknownType(); };
unsigned long CChunkedFileRawDataChunk::IsUnknownType(){ return 1; }