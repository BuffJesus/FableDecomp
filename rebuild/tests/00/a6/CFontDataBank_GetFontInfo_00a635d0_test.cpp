#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "fable_font_data_banks.h"
int main() {
    unsigned char storage[sizeof(CFontDataBank)]; memset(storage, 0xA5, sizeof(storage));
    CFontDataBank& bank=*reinterpret_cast<CFontDataBank*>(storage);
    CFontDataBankEntry* entries[3] = { reinterpret_cast<CFontDataBankEntry*>(0x1000), reinterpret_cast<CFontDataBankEntry*>(0x2000), reinterpret_cast<CFontDataBankEntry*>(0x3000) };
    void** words=reinterpret_cast<void**>(storage+offsetof(CFontDataBank, Fonts));
    words[0]=entries; words[1]=entries+3; words[2]=entries+3;
    for (unsigned i=0;i<3;++i) if (bank.GetFontInfo(i)!=entries[i]) return 1;
    puts("FONT_INFO_00a635d0_PASS");return 0;
}
