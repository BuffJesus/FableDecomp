#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "fable_filesystem.h"

// Wide strings in this fixture own a heap copy of their text in the storage slot.
namespace
{
    wchar_t*& Slot(const CWideString& s) { return *reinterpret_cast<wchar_t**>(const_cast<CWideString*>(&s)); }
    const wchar_t* Text(const CWideString& s) { return Slot(s) ? Slot(s) : L""; }
    wchar_t* Dup(const wchar_t* t)
    {
        size_t n = wcslen(t) + 1;
        wchar_t* p = (wchar_t*)malloc(n * sizeof(wchar_t));
        memcpy(p, t, n * sizeof(wchar_t));
        return p;
    }
    void Set(CWideString& s, const wchar_t* t) { free(Slot(s)); Slot(s) = t[0] ? Dup(t) : 0; }
    void Append(CWideString& s, const wchar_t* t)
    {
        wchar_t buffer[512];
        wcscpy(buffer, Text(s));
        wcscat(buffer, t);
        Set(s, buffer);
    }
    int g_absoluteCalls;
}

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const wchar_t* text) : storage_(0) { Set(*this, text); }
CWideString::CWideString(const CWideString& other) : storage_(0) { Set(*this, Text(other)); }
CWideString::~CWideString() { free(Slot(*this)); Slot(*this) = 0; }
CWideString& CWideString::operator=(const CWideString& other) { Set(*this, Text(other)); return *this; }
long CWideString::GetLength() const { return (long)wcslen(Text(*this)); }
const wchar_t* CWideString::GetAsCharArray() const { return Text(*this); }
CWideString::operator const wchar_t*() const { return Text(*this); }
const CWideString& CWideString::operator+=(const wchar_t* text) { Append(*this, text); return *this; }
const CWideString& CWideString::operator+=(wchar_t character)
{
    wchar_t text[2] = { character, 0 };
    Append(*this, text);
    return *this;
}
CWideString CWideString::GetSubString(unsigned long start, unsigned long count) const
{
    wchar_t buffer[512];
    const wchar_t* text = Text(*this);
    unsigned long length = (unsigned long)wcslen(text);
    unsigned long i = 0;
    for (; start + i < length && i < count; ++i)
        buffer[i] = text[start + i];
    buffer[i] = 0;
    return CWideString(buffer);
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{
    CWideString result(left);
    Append(result, Text(right));
    return result;
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{
    CWideString result(left);
    Append(result, right);
    return result;
}
CWideString FABLE_FASTCALL CAFile::GetAbsoluteFilePath(const CWideString& directory, const CWideString& path)
{
    ++g_absoluteCalls;
    CWideString result(L"ABS(");
    Append(result, Text(directory));
    Append(result, L"|");
    Append(result, Text(path));
    Append(result, L")");
    return result;
}

namespace
{
    bool Expect(const wchar_t* path, const wchar_t* context, const wchar_t* expected)
    {
        CWideString p(path);
        CWideString c(context);
        CWideString result = CAFile::ExpandPath(p, c);
        bool ok = wcscmp(Text(result), expected) == 0 && wcscmp(Text(p), path) == 0 && wcscmp(Text(c), context) == 0;
        if (!ok)
            wprintf(L"  ExpandPath(%s, %s) = %s, want %s\n", path, context, Text(result), expected);
        return ok;
    }
}

int main()
{
    bool passed = true;
    passed &= Expect(L"C:\\abs\\x", L"D:\\ctx", L"C:\\abs\\x");
    passed &= Expect(L"\\\\server\\x", L"D:\\ctx", L"\\\\server\\x");
    passed &= Expect(L"\\root\\x", L"D:\\ctx", L"D:\\root\\x");
    passed &= Expect(L"..\\up", L"D:\\ctx", L"ABS(D:\\ctx\\|..\\up)");
    passed &= Expect(L".\\here", L"D:\\ctx\\", L"D:\\ctx\\here");
    passed &= Expect(L"plain", L"D:\\ctx", L"D:\\ctx\\plain");
    passed &= Expect(L"plain", L"", L"plain");
    passed &= Expect(L"C:x", L"D:\\ctx", L"D:\\ctx\\C:x");
    passed &= g_absoluteCalls == 1;
    if (!passed)
    {
        printf("FABLETLC_CAFILE_EXPANDPATH_00998C90 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CAFILE_EXPANDPATH_00998C90_PASS\n");
    return 0;
}