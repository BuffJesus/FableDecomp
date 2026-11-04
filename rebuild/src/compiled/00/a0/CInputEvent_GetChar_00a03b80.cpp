// CInputEvent::GetChar (retail 0x00A03B80).
// FableWin.pdb ?GetChar@CInputEvent@@QBE_WXZ (unsigned short wchar_t in the VC7.1 build: ?GetChar@CInputEvent@@QBEGXZ).
#include "fable_input_event.h"

wchar_t CInputEvent::GetChar() const
{
    return Char;
}