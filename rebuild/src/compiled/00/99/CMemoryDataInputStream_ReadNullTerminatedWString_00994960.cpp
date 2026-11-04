#include "fable_string.h"
#include "fable_data_input_stream.h"

CWideString CMemoryDataInputStream::ReadNullTerminatedWString()
{
    unsigned long position = GetPosition();
    const wchar_t* text = reinterpret_cast<const wchar_t*>(StreamSource + position);
    CWideString value(text);
    long consumed = value.GetByteLength() + 2;
    if (consumed > 0) {
        __int64 next = GetPosition() + (unsigned long)consumed;
        if (next >= 0 && next <= 0x7fffffff) {
            long newPosition = (long)next;
            long oldPosition = StreamPos;
            CurrentSourcePtr += newPosition - oldPosition;
            SourceChunkBytesRemaining += oldPosition - newPosition;
            StreamPos = newPosition;
        }
    }
    return value;
}
