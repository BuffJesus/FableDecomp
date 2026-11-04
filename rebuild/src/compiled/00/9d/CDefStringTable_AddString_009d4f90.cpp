// CDefStringTable::AddString(const CCharString&) @ 0x009D4F90 (FableWin
// ?AddString@CDefStringTable@@QAEJABVCCharString@@@Z; CDefString::SetString
// 0x009D5060 on GDefStringTable).
#include "fable_def_string.h"
#include "fable_data_output_stream_inline.h"

const char* g_DefStringTableText_013CA824;

// The table index of the string: -1 for an empty string; a string already in
// the table is shared (and counted). A new one is appended to the stream as
// its CRC then its text (the stream starts with a 4-byte header), and its
// index is the stream offset where it starts.
long CDefStringTable::AddString(const CCharString& text)
{
    if (text.IsEmpty())
        return -1;
    if (Locked)
        Unlock();

    _STL::map<CCharString, long>::iterator it = StringMap.find(text);
    if (it != StringMap.end())
    {
        ++SharedCount;
        return it->second;
    }

    if (StringStream.GetLength() == 0)
        StringStream.WriteRepeatedByte(0, 4);
    long position = StringStream.GetLength();
    StringStream.WriteULONG(text.GetCRC());
    StringStream.WriteNullTerminatedString(text);
    StringMap[text] = position;
    ++StringCount;
    g_DefStringTableText_013CA824 = (const char*)StringStream.PeekData() + 4;
    return position;
}