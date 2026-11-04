#include "fable_def_string.h"

void CDefStringTable::Unlock()
{
    if (Locked && !ReadOnly)
    {
        const char* data = static_cast<const char*>(StringStream.PeekData());
        unsigned long length = StringStream.GetLength();
        unsigned long position = 4;
        while (position < length)
        {
            CCharString text(data + position + 4);
            StringMap[text] = position;
            position += text.GetLength() + 5;
        }
        Locked = false;
    }
}
