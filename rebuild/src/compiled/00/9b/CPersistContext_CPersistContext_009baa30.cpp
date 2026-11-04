// CPersistContext::Init(CMemoryDataInputStream&, bool) @ 0x009BAA30 (FableWin
// ?Init@CPersistContext@@QAEXAAVCMemoryDataInputStream@@_N@Z): a binary load
// from the stream, with no definition manager and no CRC check
// (CDefinitionManager::LoadDef 0x009AD6E0). The format flag is not stored.
#include "fable_persist_context.h"

void CPersistContext::Init(CMemoryDataInputStream& stream, bool safeBinaryFormat)
{
    PLoadStream = &stream;
    Mode = MODE_BINARY_LOAD;
    PSaveStream = 0;
    PDefinitionManager = 0;
    UseCRCCheck = false;
}
