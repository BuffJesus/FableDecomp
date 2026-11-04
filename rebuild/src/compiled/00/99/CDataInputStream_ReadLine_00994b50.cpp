// CDataInputStream::ReadLine() @ 0x00994B50 (FableWin
// ?ReadLine@CDataInputStream@@QAE?AVCCharString@@XZ).
#include "fable_data_input_stream.h"
#include "fable_string.h"

// Characters up to a line end ("\r\n", "\r", "\n") or a zero, which are
// consumed but not returned; a lone "\r" leaves the following character.
CCharString CDataInputStream::ReadLine()
{
    CCharString line;
    while (!IsEOF())
    {
        char character = ReadValue<char>();
        if (character == '\r')
        {
            if (!IsEOF())
            {
                char next = ReadValue<char>();
                if (next != '\n')
                    SetPosition(GetPosition() - 1);
            }
            break;
        }
        if (character == '\n' || character == 0)
            break;
        line += character;
    }
    return line;
}
