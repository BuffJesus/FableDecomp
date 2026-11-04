#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_filesystem.h"

// Retail CWideStringData: an STLport basic_string<wchar_t> (start, finish,
// end of storage) and the owner count.
struct RawWideData
{
    const wchar_t* start;
    const wchar_t* finish;
    const wchar_t* endOfStorage;
    long owners;
};

namespace
{
    int g_calcCalls;
    int g_assigns;
    int g_copies;
    RawWideData g_calcData;
    const wchar_t kCalculated[] = L"C:\\work";

    RawWideData*& Slot(const CWideString& s) { return *reinterpret_cast<RawWideData**>(const_cast<CWideString*>(&s)); }

    void Fill(RawWideData& data, const wchar_t* text)
    {
        data.start = text;
        data.finish = text + wcslen(text);
        data.endOfStorage = data.finish;
        data.owners = 1;
    }
}

CWideString g_CAFileCurrentPath_013BCA10;

int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        if (lhs[i] != rhs[i])
            return lhs[i] < rhs[i] ? -1 : 1;
    return 0;
}

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) { ++g_copies; }
CWideString::~CWideString() {}
CWideString& CWideString::operator=(const CWideString& other)
{
    ++g_assigns;
    storage_ = other.storage_;
    return *this;
}

CWideString FABLE_FASTCALL NFile::CalcCurrentPath()
{
    ++g_calcCalls;
    Fill(g_calcData, kCalculated);
    CWideString result;
    Slot(result) = &g_calcData;
    return result;
}

namespace
{
    bool Run(RawWideData* initial, bool expectCalc)
    {
        g_calcCalls = g_assigns = g_copies = 0;
        Slot(g_CAFileCurrentPath_013BCA10) = initial;
        CWideString path = CAFile::GetCurrentPath();
        RawWideData* expected = expectCalc ? &g_calcData : initial;
        return g_calcCalls == (expectCalc ? 1 : 0) &&
               g_assigns == (expectCalc ? 1 : 0) &&
               Slot(g_CAFileCurrentPath_013BCA10) == expected &&
               Slot(path) == expected;
    }
}

int main()
{
    RawWideData set;
    Fill(set, L"D:\\game");
    RawWideData empty;
    Fill(empty, L"");
    RawWideData sameLengthOther;
    Fill(sameLengthOther, L"x");

    const bool passed =
        Run(0, true) &&
        Run(&set, false) &&
        Run(&empty, true) &&
        Run(&sameLengthOther, false);
    if (!passed)
    {
        printf("FABLETLC_CAFILE_GETCURRENTPATH_00997F80 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CAFILE_GETCURRENTPATH_00997F80_PASS\n");
    return 0;
}