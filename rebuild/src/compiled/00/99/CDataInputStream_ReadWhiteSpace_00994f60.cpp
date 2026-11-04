// CDataInputStream::ReadWhiteSpace() @ 0x00994F60 (FableWin
// ?ReadWhiteSpace@CDataInputStream@@QAE?AVCCharString@@XZ).
#include "fable_data_input_stream.h"
#include "fable_string.h"

// The run of spaces, tabs, CRs and LFs at the current position; the first
// other character is left unread.
CCharString CDataInputStream::ReadWhiteSpace()
{
    CCharString text;
    while (!IsEOF())
    {
        char next = ReadValue<char>();
        if (next != ' ' && next != '\t' && next != '\r' && next != '\n')
        {
            SetPosition(GetPosition() - 1);
            break;
        }
        text += next;
    }
    return text;
}
