#include "fable_inkey_buffer.h"

void CInkeyBuffer::AddChar(wchar_t character, bool pressed)
{
    if ((LastChar + 1) % 100 == FirstChar)
        return;
    Buffer[LastChar].Char = character;
    Buffer[LastChar].Pressed = pressed;
    LastChar = (LastChar + 1) % 100;
}
