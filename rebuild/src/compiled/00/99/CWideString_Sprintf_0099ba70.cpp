// lib_wide_string.cpp, retail 0x0099BA70 (/GS) and 0x0099BB80: formatted
// assignment. Sprintf formats into an 8000-character stack buffer (Ego_r
// sbuffer), or a CArray (dbuffer) when the format is long; SprintfDynamic
// always uses the CArray. The string is replaced only when the output fit.
// UnassignString and operator=(const wchar_t*) are expanded, as in the
// other lib_wide_string units.
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include "fable_string.h"
#include "fable_carray.h"
#include "fable_stlport_vector_impl.h"

inline void CWideString::UnassignString()
{
    CWideStringData* shared = storage_;
    if (shared != 0)
    {
        if (--shared->owners <= 0)
        {
            CWideStringData* storage = storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }
}

__forceinline const CWideString& CWideString::operator=(const wchar_t* text)
{
    UnassignString();
    if (text != 0 && *text != L'\0')
        storage_ = AllocStringData(text, -1);
    return *this;
}

// Retail 0099BA70.
void CWideString::Sprintf(const wchar_t* format, ...)
{
    CArray<wchar_t> dbuffer;
    wchar_t sbuffer[8000];
    long length = wcslen(format);
    wchar_t* string;
    if (length > 4000)
    {
        dbuffer.resize(length + 4000);
        string = &dbuffer[0];
    }
    else
        string = sbuffer;
    long size = length + 4000;
    va_list args;
    va_start(args, format);
    int count = _vsnwprintf(string, size - 1, format, args);
    string[size - 1] = 0;
    if (count <= size)
        *this = string;
}

// Retail 0099BB80.
void CWideString::SprintfDynamic(const wchar_t* format, ...)
{
    CArray<wchar_t> dbuffer;
    long length = wcslen(format);
    dbuffer.resize(length + 4096);
    long size = length + 4096;
    wchar_t* string = &dbuffer[0];
    va_list args;
    va_start(args, format);
    int count = _vsnwprintf(string, size - 1, format, args);
    string[size - 1] = 0;
    if (count <= size)
        *this = string;
}
