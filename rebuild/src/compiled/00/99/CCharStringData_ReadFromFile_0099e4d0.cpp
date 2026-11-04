// lib_char_string.cpp, retail 0x0099E4D0: read `length` characters into the
// data's existing buffer (nothing for a non-positive length).
#include "fable_string.h"
#include "fable_filesystem.h"

void CCharStringData::ReadFromFile(CAFile& file, CASuspendableProcess* process, long length)
{
    if (length > 0)
        file.Read(Data.PData, length, process);
}
