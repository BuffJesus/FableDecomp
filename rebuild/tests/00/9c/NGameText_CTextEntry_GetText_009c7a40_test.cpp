#include <stdio.h>
#include <string.h>

#include "fable_game_text_bank.h"

namespace
{
    const CWideString* g_copySource = 0;
    int g_copyCalls = 0;
    int g_destructorCalls = 0;
}

CWideString::CWideString(const CWideString& other)
{
    ++g_copyCalls;
    g_copySource = &other;
    *reinterpret_cast<CWideStringData**>(this) =
        *reinterpret_cast<CWideStringData* const*>(&other);
}

CWideString::~CWideString()
{
    ++g_destructorCalls;
}

int main()
{
    fable_u8 entryBytes[sizeof(NGameText::CTextEntry)];
    memset(entryBytes, 0, sizeof(entryBytes));
    NGameText::CTextEntry* entry =
        reinterpret_cast<NGameText::CTextEntry*>(entryBytes);

    unsigned long storageRaw_[4];
    CWideStringData& storage = *reinterpret_cast<CWideStringData*>(storageRaw_);
    memset(&storage, 0, sizeof(storage));
    storage.owners = 3;
    *reinterpret_cast<CWideStringData**>(
        entryBytes + 0x28) = &storage;

    {
        CWideString handle = entry->GetText();
        if (g_copyCalls != 1)
            return 1;
        if (
            g_copySource !=
            reinterpret_cast<const CWideString*>(entryBytes + 0x28))
        {
            return 2;
        }
        if (
            *reinterpret_cast<CWideStringData* const*>(&handle) !=
            &storage)
        {
            return 3;
        }
    }

    if (g_destructorCalls != 1)
        return 4;

    puts("TEXT_ENTRY_GET_TEXT PASS");
    return 0;
}
