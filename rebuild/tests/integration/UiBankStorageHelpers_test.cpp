#include "fable_ui_bank_storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
fable_i32 g_CCharStringInstanceCount_013BD800;
static FableUiGraphicArray array;
static unsigned oldData[32],newData[32],fill,seed,mode;
static CCharStringData records[8];
struct Node { FableUiBankTreeNode Links; FableUiStringValue Key; unsigned Value; };
static Node nodes[7];
static FableUiBankTree tree;
static unsigned Normalize(unsigned v)
{
    unsigned starts[]={reinterpret_cast<unsigned>(oldData),reinterpret_cast<unsigned>(newData),reinterpret_cast<unsigned>(records),reinterpret_cast<unsigned>(nodes)};
    unsigned sizes[]={128,128,136,168};
    for(unsigned i=0;i<4;++i) if(v>=starts[i] && v<starts[i]+sizes[i]) return 0x61000000+i*0x1000000+v-starts[i];
    return v;
}
void* __cdecl FableUiAllocateArchiveArray(unsigned size)
{ printf(" ALLOC%u:%d",size,g_CCharStringInstanceCount_013BD800); if(size>128) abort(); return newData; }
void __cdecl FableUiFreeArchiveArray(void* p)
{ printf(" FREE%08x:%d",Normalize(reinterpret_cast<unsigned>(p)),g_CCharStringInstanceCount_013BD800); }
void __cdecl FableUiFreeStringRecord(void* p) { printf(" RF%u",static_cast<unsigned>(static_cast<CCharStringData*>(p)-records)); }
void __cdecl FableUiFreeStringBuffer(void*) { abort(); }
static void Dump(void* p,unsigned size)
{ for(unsigned i=0;i<size;i+=4) { unsigned v; memcpy(&v,static_cast<unsigned char*>(p)+i,4); printf(":%08x",Normalize(v)); } }
int main()
{
    for(mode=0;mode<4;++mode) for(seed=0;seed<(mode==3 ? 256u : 1024u);++seed)
    {
        memset(newData,0xCD,sizeof(newData)); memset(records,0,sizeof(records)); memset(nodes,0xA5,sizeof(nodes)); g_CCharStringInstanceCount_013BD800=100;
        for(unsigned i=0;i<32;++i) oldData[i]=seed*131+i*17; fill=0xA5B6C7D8;
        unsigned length=seed%8,spare=(seed/8)%4,count=(seed/32)%16;
        array.Begin=oldData; array.End=oldData+length; array.Capacity=oldData+length+spare;
        if(mode==2) for(i=0;i<length;++i) { unsigned index=(seed+i)%5; reinterpret_cast<FableUiStringValue*>(oldData)[i].Storage=records+index; ++records[index].owners; }
        if(mode==3)
        {
            count=seed%8;
            for(i=0;i<count;++i)
            {
                unsigned left=seed&8 ? i+1 : i*2+1,right=seed&8 ? count : i*2+2;
                nodes[i].Links.Left=left<count ? &nodes[left].Links : 0; nodes[i].Links.Right=right<count ? &nodes[right].Links : 0;
                if(seed&16) { FableUiBankTreeNode* t=nodes[i].Links.Left; nodes[i].Links.Left=nodes[i].Links.Right; nodes[i].Links.Right=t; }
                unsigned key=(i+seed)%5; nodes[i].Key.Storage=seed&32 ? 0 : records+key; if(nodes[i].Key.Storage) ++records[key].owners;
            }
        }
        printf("TRACE");
        const unsigned* value=seed&512 && length ? oldData+length-1 : &fill;
        if(mode==0) FableUiResizeBankChecksums(&array,0,count,value);
        if(mode==1) FableUiResizeBankUpdates(&array,0,count,value);
        if(mode==2) FableUiResizeBankSymbols(&array,0,count);
        if(mode==3) FableUiDestroyBankStringTree(&tree,0,count ? &nodes[0].Links : 0);
        printf(" ARRAY"); Dump(&array,12); printf(" OLD"); Dump(oldData,128); printf(" NEW"); Dump(newData,128); printf(" NODES"); Dump(nodes,sizeof(nodes));
        printf(" RECORDS"); for(i=0;i<136;++i) printf("%02x",reinterpret_cast<unsigned char*>(records)[i]); printf(" COUNT%d END\n",g_CCharStringInstanceCount_013BD800);
    }
    return 0;
}
