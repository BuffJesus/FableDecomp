// CDataInputStream::ReadNullTerminatedWString() @ 0x00994D20 (FableWin
// ?ReadNullTerminatedWString@CDataInputStream@@UAE?AVCWideString@@XZ; vtable
// slot 7): the wide twin of ReadNullTerminatedString 0x00994C80.
#include "fable_data_input_stream.h"
#include "fable_string.h"

// Characters up to (not including) the terminating zero.
CWideString CDataInputStream::ReadNullTerminatedWString()
{
    CWideString result;
    wchar_t character;
    while ((character = ReadValue<wchar_t>()) != 0)
        result += character;
    return result;
}
