#include <stdio.h>
#include <string.h>
extern long TextureLoadingPriority;
typedef char LongIsFourBytes[sizeof(long)==4?1:-1];
static unsigned long ReadPriorityBits()
{
    volatile long* value=&TextureLoadingPriority;
    return (unsigned long)*value;
}
int main()
{
    if(ReadPriorityBits()!=3)return 1;
    const unsigned long states[]={0,3,0x80000000UL,0xFFFFFFFFUL};
    for(unsigned long i=0;i<4;++i){
        memcpy(&TextureLoadingPriority,&states[i],4);
        if(ReadPriorityBits()!=states[i])return 2;
        if((states[i]&0x80000000UL)&&TextureLoadingPriority>=0)return 3;
    }
    TextureLoadingPriority=3;
    if(ReadPriorityBits()!=3)return 4;
    printf("NATIVE_TEXTURE_LOADING_PRIORITY_PASS cases=4 initial=3 writable=1 signed=1\n");
    return 0;
}
