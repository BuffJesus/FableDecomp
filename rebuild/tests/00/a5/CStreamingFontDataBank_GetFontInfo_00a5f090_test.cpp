#include <stdio.h>
#include <string.h>
#include "engine/CStreamingFontDataBank.h"
int main() {
    double storage[(sizeof(CStreamingFontDataBank)+16+7)/8];
    memset(storage, 0xA5, sizeof(storage));
    CStreamingFontDataBank* bank = reinterpret_cast<CStreamingFontDataBank*>(reinterpret_cast<char*>(storage)+8);
    CStreamingFontDataBankEntry* entries[5] = {(CStreamingFontDataBankEntry*)0x1234, 0, (CStreamingFontDataBankEntry*)0x9876, (CStreamingFontDataBankEntry*)0xF00C, (CStreamingFontDataBankEntry*)0x1234};
    bank->Fonts._M_start = entries;
    bank->Fonts._M_finish = entries+5;
    bank->Fonts._M_end_of_storage._M_data = entries+5;
    unsigned char expected[sizeof(storage)]; memcpy(expected,storage,sizeof(storage));
    for (unsigned long i=0;i<5;++i) if (static_cast<const CStreamingFontDataBank*>(bank)->PeekFontInfo(i)!=entries[i]) return 1;
    if(memcmp(expected,storage,sizeof(storage)))return 2;
    entries[2]=(CStreamingFontDataBankEntry*)0x7766;
    if(static_cast<const CStreamingFontDataBank*>(bank)->PeekFontInfo(2)!=entries[2])return 3;
    puts("FONT_00a5f090_PASS");return 0;
}
