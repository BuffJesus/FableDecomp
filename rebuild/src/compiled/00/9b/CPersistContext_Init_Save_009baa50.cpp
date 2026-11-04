// CPersistContext::Init(CDataOutputStream&, bool) @ 0x009BAA50 (FableWin
// ?Init@CPersistContext@@QAEXAAVCDataOutputStream@@_N@Z): a binary save to
// the stream, with no definition manager and no CRC check. The format flag
// is not stored.
#include "fable_persist_context.h"

void CPersistContext::Init(CDataOutputStream& stream, bool safeBinaryFormat)
{
    PSaveStream = &stream;
    Mode = MODE_BINARY_SAVE;
    PLoadStream = 0;
    PDefinitionManager = 0;
    UseCRCCheck = false;
}