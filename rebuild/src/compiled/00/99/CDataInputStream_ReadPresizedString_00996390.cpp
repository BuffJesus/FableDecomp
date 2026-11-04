// lib_data_stream.cpp, retail 0x00996390 (Ego_r
// ?ReadPresizedString@CDataInputStream@@QAE?AVCCharString@@XZ): a 32-bit
// length, then that many characters through the inline Read into a
// CArray<char> one longer for the terminator. The length comes through a
// read temporary (ReadValue), so it lives in a register. The vector(n) constructor
// (0x00411910) stays an out-of-line call, so fable_stlport_vector_impl.h is
// not included here.
#include "fable_data_input_stream.h"
#include "fable_carray.h"
#include "fable_string.h"

CCharString CDataInputStream::ReadPresizedString()
{
    long length = ReadValue<long>();
    if (length != 0)
    {
        CArray<char> buffer(length + 1);
        // The const GetAsCArray (its `sub` size test is retail's).
        const CArray<char>& chars = buffer;
        Read(const_cast<char*>(chars.GetAsCArray()), length);
        buffer[length] = 0;
        CCharString string(chars.GetAsCArray());
        return string;
    }
    return CCharString("");
}
