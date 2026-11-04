#include "fable_inkey_buffer.h"
#include <string.h>
#include <stdio.h>
int main() {
    unsigned cases=0;
    const wchar_t characters[]={0,0x41,0x7f,0x8000,0xd800,0xffff};
    for(unsigned first=0;first<100;++first) for(unsigned last=0;last<100;++last)
    for(unsigned c=0;c<6;++c) for(unsigned pressed=0;pressed<2;++pressed) {
        unsigned long storage[0x1a0/4];memset(storage,0xa5,sizeof(storage));
        CInkeyBuffer* buffer=reinterpret_cast<CInkeyBuffer*>(storage);
        buffer->FirstChar=first;buffer->LastChar=last;
        unsigned char expected[0x1a0];memcpy(expected,storage,sizeof(expected));
        unsigned next=last==99?0:last+1;
        if(next!=first) {
            memcpy(expected+8+4*last,&characters[c],2);
            expected[10+4*last]=static_cast<unsigned char>(pressed);
            memcpy(expected+0x19c,&next,4);
        }
        buffer->AddChar(characters[c],pressed!=0);++cases;
        if(memcmp(storage,expected,sizeof(expected))) { printf("INKEY_ADD_CHAR_FAIL first=%u last=%u char=%x pressed=%u\n",first,last,characters[c],pressed);return 1; }
    }
    printf("INKEY_ADD_CHAR_PASS scenarios=%u\n",cases);return 0;
}
