#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "fable_string.h"

namespace
{
    unsigned long g_newStorageRaw_[4];
    CWideStringData& g_newStorage = *reinterpret_cast<CWideStringData*>(g_newStorageRaw_);
    unsigned int g_allocateCalls;
    unsigned int g_freeCalls;
    unsigned int g_deleteCalls;
    const wchar_t* g_allocatedText;
    long g_allocatedLength;
    void* g_freedMemory;
    void* g_deletedMemory;

    void SetStorage(
        CWideString& value,
        CWideStringData* storage)
    {
        *reinterpret_cast<CWideStringData**>(&value) = storage;
    }

    CWideStringData* GetStorage(CWideString& value)
    {
        return *reinterpret_cast<CWideStringData**>(&value);
    }

    void ResetObservations()
    {
        memset(&g_newStorage, 0, sizeof(g_newStorage));
        g_allocateCalls = 0;
        g_freeCalls = 0;
        g_deleteCalls = 0;
        g_allocatedText = 0;
        g_allocatedLength = 0;
        g_freedMemory = 0;
        g_deletedMemory = 0;
    }

    bool CheckSharedReplacement()
    {
        ResetObservations();
        wchar_t oldText[] = L"old";
        unsigned long oldStorageRaw_[4] = {reinterpret_cast<unsigned long>(oldText), 0, 0, 2};
        CWideStringData& oldStorage = *reinterpret_cast<CWideStringData*>(oldStorageRaw_);
        fable_u8 valueBytes[sizeof(CWideString)] = {0};
        CWideString& value =
            *reinterpret_cast<CWideString*>(valueBytes);
        SetStorage(value, &oldStorage);

        const CWideString& result = (value = L"new");
        return
            &result == &value &&
            oldStorage.owners == 1 &&
            g_freeCalls == 0 &&
            g_deleteCalls == 0 &&
            g_allocateCalls == 1 &&
            g_allocatedLength == -1 &&
            wcscmp(g_allocatedText, L"new") == 0 &&
            GetStorage(value) == &g_newStorage;
    }

    bool CheckLastOwnerAndEmpty()
    {
        ResetObservations();
        wchar_t oldText[] = L"old";
        unsigned long oldStorageRaw_[4] = {reinterpret_cast<unsigned long>(oldText), 0, 0, 1};
        CWideStringData& oldStorage = *reinterpret_cast<CWideStringData*>(oldStorageRaw_);
        fable_u8 valueBytes[sizeof(CWideString)] = {0};
        CWideString& value =
            *reinterpret_cast<CWideString*>(valueBytes);
        SetStorage(value, &oldStorage);

        value = L"";
        return
            oldStorage.owners == 0 &&
            g_freeCalls == 1 &&
            g_freedMemory == oldText &&
            g_deleteCalls == 1 &&
            g_deletedMemory == &oldStorage &&
            g_allocateCalls == 0 &&
            GetStorage(value) == 0;
    }

    bool CheckNullInput()
    {
        ResetObservations();
        fable_u8 valueBytes[sizeof(CWideString)] = {0};
        CWideString& value =
            *reinterpret_cast<CWideString*>(valueBytes);
        value = static_cast<const wchar_t*>(0);
        return
            g_allocateCalls == 0 &&
            g_freeCalls == 0 &&
            g_deleteCalls == 0 &&
            GetStorage(value) == 0;
    }
}

// The fixture owns every buffer, so the CRT release routines only record.
extern "C" void __cdecl free(void* memory)
{
    ++g_freeCalls;
    g_freedMemory = memory;
}

void operator delete(void* memory)
{
    ++g_deleteCalls;
    g_deletedMemory = memory;
}

CWideStringData* CWideString::AllocStringData(
    const wchar_t* text,
    long length)
{
    ++g_allocateCalls;
    g_allocatedText = text;
    g_allocatedLength = length;
    g_newStorage.text = const_cast<wchar_t*>(text);
    g_newStorage.owners = 1;
    return &g_newStorage;
}

int main()
{
    const bool passed =
        CheckSharedReplacement() &&
        CheckLastOwnerAndEmpty() &&
        CheckNullInput();
    if (!passed)
    {
        printf("FABLETLC_WIDE_STRING_ASSIGN_LITERAL FAIL\n");
        return 1;
    }

    printf("FABLETLC_WIDE_STRING_ASSIGN_LITERAL PASS\n");
    return 0;
}
