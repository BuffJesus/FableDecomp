// CDataInputStream::ReadUntilWhiteSpace() @ 0x00994E90 (FableWin
// ?ReadUntilWhiteSpace@CDataInputStream@@QAE?AVCCharString@@XZ).
#include "fable_data_input_stream.h"
#include "fable_string.h"

// Characters up to the next space, tab, CR or LF, which is left unread.
CCharString CDataInputStream::ReadUntilWhiteSpace()
{
    CCharString text;
    while (!IsEOF())
    {
        char next = ReadValue<char>();
        if (next == ' ' || next == '\t' || next == '\r' || next == '\n')
        {
            SetPosition(GetPosition() - 1);
            break;
        }
        text += next;
    }
    return text;
}
