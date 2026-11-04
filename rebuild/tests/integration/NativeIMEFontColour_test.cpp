#include <stdio.h>
#include <string.h>
#include "fable_ime.h"
extern unsigned char g_IMEAlphaDefault_013750B0;

int main()
{
    if(g_IMEAlphaDefault_013750B0!=255)return 1;
    const unsigned long colours[]={0,0xffffffff,0xff000000,0x00ffffff,
        0x12345678,0x87654321,0x80008000,0x00800080,0x7fffffff};
    unsigned cases=0;
    for(unsigned alpha=0;alpha<256;++alpha)for(unsigned i=0;i<9;++i) {
        unsigned char storage[sizeof(NUISystem::CIMEFont)],expected[sizeof(storage)];
        memset(storage,(alpha+i*37)&255,sizeof(storage));memcpy(expected,storage,sizeof(storage));
        const unsigned long packed=(colours[i]&0x00ffffff)|(alpha<<24);
        memcpy(expected+4,&packed,4);g_IMEAlphaDefault_013750B0=(unsigned char)alpha;
        ((NUISystem::CIMEFont*)storage)->NUISystem::CIMEFont::SetColor(colours[i]);
        if(memcmp(storage,expected,sizeof(storage)) || g_IMEAlphaDefault_013750B0!=alpha)return 2;
        ++cases;
    }
    printf("NATIVE_IME_FONT_COLOUR_PASS cases=%u\n",cases);return 0;
}
