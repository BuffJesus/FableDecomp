// CDefStringTable::ReadTable(CDataInputStream&) @ 0x009D4AB0 (FableWin
// ?ReadTable@CDefStringTable@@QAEXAAVCDataInputStream@@@Z): loads names.bin. A
// header word is skipped; the table's RandomID (which compiled def files must
// match) and string count follow, then the size of the string data, which is
// copied into the table's stream; the text starts past its 4-byte header.
#include "fable_def_string.h"
#include "fable_data_input_stream.h"

void CDefStringTable::ReadTable(CDataInputStream& stream)
{
    stream.ReadValue<unsigned long>();
    RandomID = stream.ReadValue<unsigned long>();
    StringCount = stream.ReadValue<unsigned long>();
    unsigned long size = stream.ReadValue<unsigned long>();
    StringStream.ReserveSize(size);
    StringStream.CopyFrom(stream, size);
    g_DefStringTableText_013CA824 = static_cast<const char*>(StringStream.PeekData()) + 4;
}