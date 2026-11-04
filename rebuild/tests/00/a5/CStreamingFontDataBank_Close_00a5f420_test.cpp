#include <stdio.h>
#include <string.h>
#include "fable_font_data_banks.h"

static char g_calls[4];
static unsigned g_count;
void CStreamingFontDataBank::CloseFonts() { g_calls[g_count++] = 'F'; }
void CBankFile::Close() { g_calls[g_count++] = 'B'; }

int main()
{
    unsigned char storage[sizeof(CStreamingFontDataBank)];
    memset(storage, 0, sizeof(storage));
    CStreamingFontDataBank* bank =
        reinterpret_cast<CStreamingFontDataBank*>(storage);
    bank->CStreamingFontDataBank::Close();
    if (g_count != 2 || g_calls[0] != 'F' || g_calls[1] != 'B')
        return 1;
    puts("STREAMING_FONT_BANK_CLOSE_00a5f420_PASS");
    return 0;
}
