// Behaviour: GetHeaderDir copy-constructs its result (hidden return slot) from
// the stored bank header directory (0x013CA7D4) exactly once.
#include "fable_bank_file.h"
#include <stdio.h>

extern "C" fable_u8 g_FableBankHeaderDirectory_013CA7D4[sizeof(CWideString)];
fable_u8 g_FableBankHeaderDirectory_013CA7D4[sizeof(CWideString)];

static int g_copies; static const void* g_this; static const void* g_from;
CWideString::CWideString(const CWideString& other) { ++g_copies; g_this = this; g_from = &other; }
CWideString::~CWideString() {}

int main()
{
    {
        CWideString result = NBankFileManager::GetHeaderDir();
        if (g_copies != 1 || g_this != &result || g_from != g_FableBankHeaderDirectory_013CA7D4) { puts("FAIL"); return 1; }
    }
    puts("FABLETLC_NBANKFILEMANAGER_GETHEADERDIR_009A76A0_PASS");
    return 0;
}
