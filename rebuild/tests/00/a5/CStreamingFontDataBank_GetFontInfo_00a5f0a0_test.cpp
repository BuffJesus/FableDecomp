#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "fable_font_data_banks.h"
int main() {
    unsigned char storage[sizeof(CStreamingFontDataBank)]; memset(storage, 0xA5, sizeof(storage));
    CStreamingFontDataBank& bank=*reinterpret_cast<CStreamingFontDataBank*>(storage);
    CStreamingFontDataBankEntry* entries[3] = { reinterpret_cast<CStreamingFontDataBankEntry*>(0x1000), reinterpret_cast<CStreamingFontDataBankEntry*>(0x2000), reinterpret_cast<CStreamingFontDataBankEntry*>(0x3000) };
    void** words=reinterpret_cast<void**>(storage+offsetof(CStreamingFontDataBank, Fonts));
    words[0]=entries; words[1]=entries+3; words[2]=entries+3;
    for (unsigned i=0;i<3;++i) if (bank.GetFontInfo(i)!=entries[i]) return 1;
    puts("FONT_INFO_00a5f0a0_PASS");return 0;
}
