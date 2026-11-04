// lib_file_abstract.cpp, retail 0x00997BB0 (Ego_r
// ?GetCopyFilename@CAFile@@SI?AVCCharString@@ABV2@@Z and its locals): the name
// for a copy of a file. "name.ext" becomes "name2.ext", and a trailing number
// is incremented ("name7.ext" becomes "name8.ext"). The number is read digit by
// digit from the end, each digit scaled by pow(10, position) through a float.
#include <math.h>
#include "fable_filesystem.h"
#include "fable_string.h"

CCharString FABLE_FASTCALL CAFile::GetCopyFilename(const CCharString& old_name)
{
    CCharString basic_name(old_name);
    CCharString extension("");
    long length = old_name.GetLength();
    for (long i = 1; i <= length; ++i)
    {
        if (basic_name.GetConstCharPtr()[length - i] == '.')
        {
            basic_name = old_name.GetSubString(0, length - i);
            extension = old_name.GetSubString(length - i, length);
            break;
        }
    }
    long basic_length = basic_name.GetLength();
    long suffix = 0;
    for (long j = 1; j <= basic_length; ++j)
    {
        CCharString current_suffix = basic_name.GetSubString(basic_length - j, basic_length - j + 1);
        if (!GFIsCharStringNumber(current_suffix))
            break;
        suffix += GFCharStringToInt(current_suffix) * (float)pow(10.0, (double)(j - 1));
    }
    if (suffix == 0)
        return basic_name + GFIntToCharString(2) + extension;
    else
    {
        CCharString suffix_string = GFIntToCharString(suffix);
        CCharString new_name = basic_name.GetSubString(0, basic_length - suffix_string.GetLength());
        new_name += GFIntToCharString(suffix + 1);
        return new_name + extension;
    }
}
