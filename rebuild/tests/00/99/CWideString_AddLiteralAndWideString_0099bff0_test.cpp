#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "fable_string.h"
#include <stdlib.h>

// The unit's inline AllocStringData/MakeOwnCopyOfData copies now construct
// CWideStringData through STLport, whose length-error path (retail 0x0099C550)
// is out of line; these paths never reach it.
void _STL::_String_base<wchar_t, _STL::allocator<wchar_t> >::_M_throw_length_error() const
{
    abort();
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;
wchar_t g_FableEmptyWideString_0129A8E0[1] = {L'\0'};

namespace
{
    unsigned int g_newCalls;
    unsigned int g_deleteCalls;
    // Copies and appends go through the real STLport range initialiser
    // (0x0099C670) and append (0x0099C7E0), so they are checked by content.
    unsigned int g_appendCalls;

    CWideStringData* GetStorage(const CWideString& value)
    {
        return *reinterpret_cast<CWideStringData* const*>(&value);
    }

    // A CWideString whose storage the fixture owns, never destroyed.
    CWideString& MakeString(fable_u8* bytes, CWideStringData* storage)
    {
        *reinterpret_cast<CWideStringData**>(bytes) = storage;
        return *reinterpret_cast<CWideString*>(bytes);
    }

    wchar_t* HeapText(const wchar_t* text)
    {
        const size_t length = wcslen(text);
        wchar_t* copy =
            static_cast<wchar_t*>(malloc((length + 1) * sizeof(wchar_t)));
        memcpy(copy, text, (length + 1) * sizeof(wchar_t));
        return copy;
    }

    void Reset()
    {
        g_newCalls = 0;
        g_deleteCalls = 0;
        g_appendCalls = 0;
    }

    bool TextIs(const CWideString& value, const wchar_t* expected)
    {
        const CWideStringData* storage = GetStorage(value);
        return storage != 0 && storage->text != 0 &&
            wcscmp(storage->text, expected) == 0;
    }

    // Both sides populated: a fresh "leftright" is built and right is untouched.
    bool CheckJoin()
    {
        Reset();
        unsigned long rightStorageRaw_[4] = {reinterpret_cast<unsigned long>(HeapText(L"right")), 0, 0, 1};
        CWideStringData& rightStorage = *reinterpret_cast<CWideStringData*>(rightStorageRaw_);
        fable_u8 rightBytes[sizeof(CWideString)];
        const CWideString& right = MakeString(rightBytes, &rightStorage);
        g_CWideStringInstanceCount_013BCA20 = 20;

        bool passed;
        {
            CWideString result = L"left" + right;
            passed =
                TextIs(result, L"leftright") &&
                GetStorage(result) != &rightStorage &&
                GetStorage(result)->owners == 1 &&
                wcscmp(rightStorage.text, L"right") == 0 &&
                rightStorage.owners == 1 &&
                g_newCalls == 1 &&
                g_deleteCalls == 0 &&
                g_CWideStringInstanceCount_013BCA20 == 21;
        }
        return passed &&
            g_deleteCalls == 1 &&
            g_CWideStringInstanceCount_013BCA20 == 20;
    }

    // An empty literal shares the right side's data.
    bool CheckEmptyLeft()
    {
        Reset();
        unsigned long rightStorageRaw_[4] = {reinterpret_cast<unsigned long>(HeapText(L"right")), 0, 0, 1};
        CWideStringData& rightStorage = *reinterpret_cast<CWideStringData*>(rightStorageRaw_);
        fable_u8 rightBytes[sizeof(CWideString)];
        const CWideString& right = MakeString(rightBytes, &rightStorage);
        g_CWideStringInstanceCount_013BCA20 = 0;

        bool passed;
        {
            CWideString result = L"" + right;
            passed =
                GetStorage(result) == &rightStorage &&
                rightStorage.owners == 2 &&
                g_newCalls == 0 &&
                g_CWideStringInstanceCount_013BCA20 == 1;
        }
        return passed &&
            rightStorage.owners == 1 &&
            g_deleteCalls == 0 &&
            g_CWideStringInstanceCount_013BCA20 == 0;
    }

    // An empty right side appends the shared empty text.
    bool CheckEmptyRight()
    {
        Reset();
        fable_u8 rightBytes[sizeof(CWideString)];
        const CWideString& right = MakeString(rightBytes, 0);
        g_CWideStringInstanceCount_013BCA20 = 0;

        bool passed;
        {
            CWideString result = L"left" + right;
            passed =
                TextIs(result, L"left") &&
                GetStorage(result)->owners == 1 &&
                g_newCalls == 1 &&
                g_CWideStringInstanceCount_013BCA20 == 1;
        }
        return passed &&
            g_deleteCalls == 1 &&
            g_CWideStringInstanceCount_013BCA20 == 0;
    }
}

void* operator new(size_t size)
{
    ++g_newCalls;
    return malloc(size);
}

void operator delete(void* memory)
{
    if (memory != 0)
        ++g_deleteCalls;
    free(memory);
}


int main()
{
    const bool passed =
        CheckJoin() &&
        CheckEmptyLeft() &&
        CheckEmptyRight();
    if (!passed)
    {
        printf("FABLETLC_WIDE_STRING_ADD_LEFT FAIL\n");
        return 1;
    }

    printf("FABLETLC_WIDE_STRING_ADD_LEFT PASS\n");
    return 0;
}
