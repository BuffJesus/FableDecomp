#include <stdio.h>
#include <string.h>
#include "engine/CFontDataBank.h"
int main() {
    double storage[(sizeof(CFontDataBank)+16+7)/8];
    memset(storage, 0xA5, sizeof(storage));
    CFontDataBank* bank = reinterpret_cast<CFontDataBank*>(reinterpret_cast<char*>(storage)+8);
    CFontDataBankEntry* entries[5] = {(CFontDataBankEntry*)0x1234, 0, (CFontDataBankEntry*)0x9876, (CFontDataBankEntry*)0xF00C, (CFontDataBankEntry*)0x1234};
    bank->Fonts._M_start = entries;
    bank->Fonts._M_finish = entries+5;
    bank->Fonts._M_end_of_storage._M_data = entries+5;
    unsigned char expected[sizeof(storage)]; memcpy(expected,storage,sizeof(storage));
    for (unsigned long i=0;i<5;++i) if (static_cast<const CFontDataBank*>(bank)->PeekFontInfo(i)!=entries[i]) return 1;
    if(memcmp(expected,storage,sizeof(storage)))return 2;
    entries[2]=(CFontDataBankEntry*)0x7766;
    if(static_cast<const CFontDataBank*>(bank)->PeekFontInfo(2)!=entries[2])return 3;
    puts("FONT_00a635e0_PASS");return 0;
}
