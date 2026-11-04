// lib_file_abstract.cpp, retail 0x00998F60 (Ego_r
// ?CompareFiles@CAFile@@SI_NAAV1@0@Z and its locals): rewind both files,
// compare lengths, then compare the contents in 4 KB chunks.
#include <string.h>
#include "fable_carray.h"
#include "fable_filesystem.h"
#include "fable_stlport_vector_impl.h"

bool FABLE_FASTCALL CAFile::CompareFiles(CAFile& src_file, CAFile& dst_file)
{
    src_file.SetPosition(0);
    dst_file.SetPosition(0);
    if (src_file.GetLength() != dst_file.GetLength())
        return false;
    const long max_bytes_to_check = 0x1000;
    CArray<unsigned char> src_buffer(max_bytes_to_check);
    CArray<unsigned char> dst_buffer(max_bytes_to_check);
    long bytes_to_check = src_file.GetLength();
    while (bytes_to_check > 0)
    {
        long count = _STL::min(bytes_to_check, max_bytes_to_check);
        src_file.Read(&src_buffer[0], count, 0);
        dst_file.Read(&dst_buffer[0], count, 0);
        if (memcmp(&src_buffer[0], &dst_buffer[0], count) != 0)
            return false;
        bytes_to_check -= count;
    }
    return true;
}
