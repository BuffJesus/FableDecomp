// CMemoryDataOutputStream::CMemoryDataOutputStream() @ 0x00996AF0 (FableWin
// ??0CMemoryDataOutputStream@@QAE@XZ; CDefStringTable 0x009D4DB0 builds its
// string stream with it). The base ctors are inline except
// CBaseClassNonCopyable's (0x0099A2F0).
#include "fable_data_output_stream.h"

// An empty stream at position 0.
CMemoryDataOutputStream::CMemoryDataOutputStream()
    : CurrentPosition(0)
{
}