#include "fable_ui_bank_aliases.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
fable_i32 g_CCharStringInstanceCount_013BD800;
static FableUiBankAliases target,source;
static unsigned oldData[257],sourceData[257],newData[257],seed,mode;
static CCharStringData records[8];
static unsigned Normalize(unsigned v)
{
    unsigned starts[]={reinterpret_cast<unsigned>(oldData),reinterpret_cast<unsigned>(sourceData),reinterpret_cast<unsigned>(newData),reinterpret_cast<unsigned>(records)};
    unsigned sizes[]={1028,1028,1028,136};
    for(unsigned i=0;i<4;++i) if(v>=starts[i] && v<starts[i]+sizes[i]) return 0x61000000+i*0x1000000+v-starts[i];
    return v;
}
void* __cdecl FableUiAllocateStringBuffer(unsigned size)
{ printf(" ALLOC%u:%d",size,g_CCharStringInstanceCount_013BD800); if(size>1028) abort(); return mode==1 && (seed&256) ? 0 : newData; }
void __cdecl FableUiFreeStringBuffer(void* p)
{ printf(" FREE%08x:%d",Normalize(reinterpret_cast<unsigned>(p)),g_CCharStringInstanceCount_013BD800); }
void __cdecl FableUiFreeStringRecord(void* p) { printf(" RF%u",static_cast<unsigned>(static_cast<CCharStringData*>(p)-records)); }
static void Dump(void* p,unsigned size)
{ for(unsigned i=0;i<size;i+=4) { unsigned v; memcpy(&v,static_cast<unsigned char*>(p)+i,4); printf(":%08x",Normalize(v)); } }
static void Setup(FableUiBankAliases* list,unsigned* data,unsigned count,unsigned salt)
{
    list->Data=reinterpret_cast<FableUiStringValue*>(data+1); list->Count=static_cast<signed char>(count); *data=count;
    for(unsigned i=0;i<count;++i)
    {
        CCharStringData* rec=(seed+i+salt)%4==0 ? 0 : records+(seed+i+salt)%8;
        list->Data[i].Storage=rec; if(rec) ++rec->owners;
    }
}
int main()
{
    for(mode=0;mode<3;++mode) for(seed=0;seed<1024;++seed)
    {
        memset(oldData,0xA5,sizeof(oldData)); memset(sourceData,0xA6,sizeof(sourceData)); memset(newData,0xCD,sizeof(newData));
        memset(records,0,sizeof(records)); memset(&target,0xB1,sizeof(target)); memset(&source,0xB2,sizeof(source));
        g_CCharStringInstanceCount_013BD800=1000;
        Setup(&target,oldData,seed%16,0); Setup(&source,sourceData,(seed/16)%16,3);
        if(mode==0) target.Count=static_cast<signed char>(seed/4); // Clear uses allocation cookie.
        if(seed&512) { target.Data=0; target.Count=0; }
        printf("TRACE");
        if(mode==0) FableUiClearBankAliases(&target,0);
        if(mode==1) FableUiResizeBankAliases(&target,0,seed%256);
        if(mode==2) FableUiCopyBankAliases(&target,0,seed&256 ? &target : &source);
        printf(" TARGET"); Dump(&target,8); printf(" SOURCE"); Dump(&source,8);
        printf(" OLD"); Dump(oldData,sizeof(oldData)); printf(" SRC"); Dump(sourceData,sizeof(sourceData)); printf(" NEW"); Dump(newData,sizeof(newData));
        printf(" RECORDS"); for(unsigned i=0;i<136;++i) printf("%02x",reinterpret_cast<unsigned char*>(records)[i]);
        printf(" COUNT%d END\n",g_CCharStringInstanceCount_013BD800);
    }
    return 0;
}
