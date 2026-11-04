#include <stdio.h>
#include <string.h>
#include "fable_font_data_banks.h"

static char g_calls[4];
static unsigned g_count;
void CFontDataBank::CloseFonts() { g_calls[g_count++] = 'F'; }
void CBankFile::Close() { g_calls[g_count++] = 'B'; }

int main()
{
    unsigned char storage[sizeof(CFontDataBank)];
    memset(storage, 0, sizeof(storage));
    CFontDataBank* bank = reinterpret_cast<CFontDataBank*>(storage);
    bank->CFontDataBank::Close();
    if (g_count != 2 || g_calls[0] != 'F' || g_calls[1] != 'B')
        return 1;
    puts("FONT_DATA_BANK_CLOSE_00a635c0_PASS");
    return 0;
}
