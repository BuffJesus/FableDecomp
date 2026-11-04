// CDataInputStream::ReadUntilCharacter(char, bool&) @ 0x00994DC0 (FableWin
// ?ReadUntilCharacter@CDataInputStream@@QAE?AVCCharString@@DAA_N@Z).
#include "fable_data_input_stream.h"
#include "fable_string.h"

// Characters before `character`, which is left unread; `found` says whether
// it was reached before the end of the stream.
CCharString CDataInputStream::ReadUntilCharacter(char character, bool& found)
{
    CCharString text;
    for (;;)
    {
        if (IsEOF())
        {
            found = false;
            break;
        }
        char next = ReadValue<char>();
        if (next == character)
        {
            SetPosition(GetPosition() - 1);
            found = true;
            break;
        }
        text += next;
    }
    return text;
}
