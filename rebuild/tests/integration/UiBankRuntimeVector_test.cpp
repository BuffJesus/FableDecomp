#include "fable_ui_bank_storage.h"
#include <stdio.h>
#include <string.h>
static FableUiGraphicArray array;
static FableUiBankRuntimeEntry oldData[24],newData[32],fill;
static unsigned seed;
void* __cdecl FableUiAllocateArchiveArray(unsigned size)
{ printf(" ALLOC%u:%u",size,static_cast<unsigned>(static_cast<FableUiBankRuntimeEntry*>(array.End)-oldData)); return newData; }
void __cdecl FableUiFreeArchiveArray(void* p)
{ printf(" FREE%u",p==oldData); if(seed&256) array.Begin=array.End=array.Capacity=0; }
static unsigned Normalize(void* p)
{
    unsigned v=reinterpret_cast<unsigned>(p),a=reinterpret_cast<unsigned>(oldData),b=reinterpret_cast<unsigned>(newData);
    if(v>=a && v<a+sizeof(oldData)) return 0x61000000+v-a;
    if(v>=b && v<b+sizeof(newData)) return 0x62000000+v-b;
    return v;
}
int main()
{
    for(seed=0;seed<1024;++seed)
    {
        unsigned length=seed%8,spare=(seed/8)%4,count=(seed/32)%16;
        memset(newData,0xCD,sizeof(newData));
        for(unsigned i=0;i<sizeof(oldData);++i) reinterpret_cast<unsigned char*>(oldData)[i]=static_cast<unsigned char>(i*7+seed);
        memset(&fill,0xA5,sizeof(fill)); array.Begin=oldData; array.End=oldData+length; array.Capacity=oldData+length+spare;
        const FableUiBankRuntimeEntry* value=(seed&512) && length ? oldData+length-1 : &fill;
        printf("TRACE"); FableUiResizeBankRuntime(&array,0,count,value);
        printf(" ARRAY:%08x:%08x:%08x OLD",Normalize(array.Begin),Normalize(array.End),Normalize(array.Capacity));
        for(i=0;i<sizeof(oldData);++i) printf("%02x",reinterpret_cast<unsigned char*>(oldData)[i]);
        printf(" NEW"); for(i=0;i<sizeof(newData);++i) printf("%02x",reinterpret_cast<unsigned char*>(newData)[i]);
        printf(" END\n");
    }
    return 0;
}
