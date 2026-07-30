#include <stdio.h>
#include <string.h>

#include "fable_bank_file.h"

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
    fable_u8 bankBytes[sizeof(CBankFileRecovered)];
    memset(bankBytes, 0, sizeof(bankBytes));
    CBankFileRecovered* bank =
        reinterpret_cast<CBankFileRecovered*>(bankBytes);

    CWideStringData storage;
    memset(&storage, 0, sizeof(storage));
    storage.owners = 3;
    *reinterpret_cast<CWideStringData**>(
        bankBytes + 0x28) = &storage;

    {
        CWideString handle = bank->GetBankHandle();
        if (g_copyCalls != 1)
            return 1;
        if (
            g_copySource !=
            reinterpret_cast<const CWideString*>(bankBytes + 0x28))
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

    puts("FABLETLC_BANK_HANDLE_BEHAVIOR PASS");
    return 0;
}
