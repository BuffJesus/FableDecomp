#include "fable_def_string.h"

void CDefStringTable::WriteTable(CDataOutputStream& output)
{
    output.WriteULONG(0x7ab1e);
    output.WriteULONG(RandomID);
    output.WriteULONG(StringCount);
    unsigned long length = StringStream.GetLength();
    output.WriteULONG(length);
    output.Write(StringStream.PeekData(), length);
}
