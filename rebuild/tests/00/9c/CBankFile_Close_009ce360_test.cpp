#include <stdio.h>
#include <string.h>
#include "fable_game_text_bank.h"

static CBankFile* g_expected;
static unsigned g_calls;
void __fastcall CloseBankFileImpl(CBankFile* bank)
{
    if (bank == g_expected)
        ++g_calls;
}
int main()
{
    unsigned char storage[sizeof(CBankFile)];
    memset(storage, 0, sizeof(storage));
    g_expected = reinterpret_cast<CBankFile*>(storage);
    g_expected->CBankFile::Close();
    if (g_calls != 1)
        return 1;
    puts("BANK_FILE_CLOSE_009ce360_PASS");
    return 0;
}
