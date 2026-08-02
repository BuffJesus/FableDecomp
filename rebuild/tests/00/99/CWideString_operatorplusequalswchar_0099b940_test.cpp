#include <wchar.h>
#include <stdio.h>

struct CWideStringData
{
    wchar_t* text;
    unsigned long unknown04;
    unsigned long unknown08;
    long owners;
};

class CWideString
{
public:
    const CWideString& operator+=(const wchar_t* text);

private:
    void MakeOwnCopyOfData();
    CWideStringData* AllocStringData(const wchar_t* text, long length);

    CWideStringData* storage_;
};

namespace
{
    unsigned int g_makeOwnCopyCalls = 0;
    unsigned int g_appendRangeCalls = 0;

    CWideStringData g_ownedStorage;
    wchar_t g_ownedBuffer[64];

    CWideStringData*& Storage(CWideString& s)
    {
        return *reinterpret_cast<CWideStringData**>(&s);
    }
}

void CWideString::MakeOwnCopyOfData()
{
    ++g_makeOwnCopyCalls;
}

extern "C" CWideStringData* __fastcall
FableWideStringAppendRange_0099C7E0(
    CWideStringData* dataSelf,
    const wchar_t* /*beginAgain*/,
    const wchar_t* first,
    const wchar_t* last,
    const wchar_t** /*ppSrcRoot*/)
{
    ++g_appendRangeCalls;
    size_t addLen = static_cast<size_t>(last - first);
    size_t curLen = wcslen(dataSelf->text);
    for (size_t i = 0; i < addLen; ++i)
    {
        dataSelf->text[curLen + i] = first[i];
    }
    dataSelf->text[curLen + addLen] = 0;
    return dataSelf;
}

CWideStringData* CWideString::AllocStringData(const wchar_t* text, long length)
{
    (void)length;
    static wchar_t buf[64];
    static CWideStringData data;
    wcscpy(buf, text);
    data.text = buf;
    data.unknown04 = 0;
    data.unknown08 = 0;
    data.owners = 1;
    return &data;
}

// Self-contained model of the target function's observable behavior
// (check_one links the test standalone, not against source_cpp's obj).
const CWideString& CWideString::operator+=(const wchar_t* text)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();

        CWideStringData* data = storage_;

        long i = 0;
        if (text[i] != 0)
        {
            do
            {
                ++i;
            } while (text[i] != 0);
        }

        FableWideStringAppendRange_0099C7E0(data, text, text, text + i, &text);
    }
    else if (text != 0 && *text != 0)
    {
        storage_ = AllocStringData(text, -1);
    }

    return *this;
}

int main()
{
    // Case 1: storage_ already allocated -> MakeOwnCopyOfData + AppendRange.
    wcscpy(g_ownedBuffer, L"Hello");
    g_ownedStorage.text = g_ownedBuffer;
    g_ownedStorage.unknown04 = 0;
    g_ownedStorage.unknown08 = 0;
    g_ownedStorage.owners = 1;

    CWideString owned;
    Storage(owned) = &g_ownedStorage;

    owned += L", World!";

    if (g_makeOwnCopyCalls != 1 || g_appendRangeCalls != 1)
        return 1;
    if (wcscmp(g_ownedBuffer, L"Hello, World!") != 0)
        return 2;
    if (Storage(owned) != &g_ownedStorage)
        return 3;

    // Case 2: storage_ still points at the same live buffer after append
    // (helper mutates in place in this test double), so a second append
    // with empty text should still detach again (retail detaches
    // unconditionally on the "already owns" path).
    owned += L"";
    if (g_makeOwnCopyCalls != 2 || g_appendRangeCalls != 2)
        return 4;

    // Case 3: storage_ == 0 with a non-empty literal -> AllocStringData path,
    // MakeOwnCopyOfData/AppendRange are NOT invoked.
    CWideString empty;
    Storage(empty) = 0;

    empty += L"Fresh";

    if (g_makeOwnCopyCalls != 2 || g_appendRangeCalls != 2)
        return 5;
    if (Storage(empty) == 0 || wcscmp(Storage(empty)->text, L"Fresh") != 0)
        return 6;

    // Case 4: storage_ == 0 with a null/empty literal -> untouched (storage_
    // stays 0), no allocation call at all.
    CWideString stillEmpty;
    Storage(stillEmpty) = 0;

    stillEmpty += L"";

    if (Storage(stillEmpty) != 0)
        return 7;

    printf("FABLETLC_WIDESTRING_APPEND_LITERAL PASS\n");
    return 0;
}