#include "fable_ui_bank_storage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
fable_i32 g_CCharStringInstanceCount_013BD800;
static FableUiBankFileView bank;
static unsigned char oldData[4][128],newData[4][128];
static CCharStringData records[8];
static FableUiBankTreeNode heads[4];
struct Node { FableUiBankTreeNode Links; FableUiStringValue Key; unsigned Value; };
static Node nodes[6];
static unsigned seed,allocated;
static unsigned Id(void* p,void* base,unsigned stride) { return p ? (static_cast<unsigned char*>(p)-static_cast<unsigned char*>(base))/stride : 99; }
void __cdecl FableUiFreeStringRecord(void* p) { printf(" RF%u",Id(p,records,17)); }
void __cdecl FableUiFreeStringBuffer(void*) { abort(); }
void __cdecl FableUiFreeArchiveArray(void* p)
{
    unsigned v=reinterpret_cast<unsigned>(p),base=reinterpret_cast<unsigned>(nodes);
    if(v>=base && v<base+sizeof(nodes))
    { unsigned id=(v-base)/24; printf(" NF%u",id); if(seed&128) { if(id<3) bank.SymbolIndices.Head=heads+2; else bank.FilenameIndices.Head=heads+3; } return; }
    unsigned id=Id(p,oldData,128); printf(" FREE%u",id); if(id==2 && (seed&64)) bank.OpenFlags^=0x38;
}
void* __cdecl FableUiAllocateArchiveArray(unsigned size)
{ printf(" ALLOC%u",size); if(!allocated && (seed&256)) bank.OpenFlags^=0x18; if(allocated>=4 || size>128) abort(); return newData[allocated++]; }
static unsigned Normalize(unsigned v)
{
    unsigned starts[]={reinterpret_cast<unsigned>(oldData),reinterpret_cast<unsigned>(newData),reinterpret_cast<unsigned>(records),reinterpret_cast<unsigned>(heads),reinterpret_cast<unsigned>(nodes)};
    unsigned sizes[]={512,512,136,64,144};
    for(unsigned i=0;i<5;++i) if(v>=starts[i] && v<starts[i]+sizes[i]) return 0x61000000+i*0x1000000+v-starts[i];
    return v;
}
static void Dump(void* p,unsigned size)
{ for(unsigned i=0;i<size;i+=4) { unsigned v; memcpy(&v,static_cast<unsigned char*>(p)+i,4); printf(":%08x",Normalize(v)); } }
int main()
{
    for(unsigned mode=0;mode<2;++mode) for(seed=0;seed<512;++seed)
    {
        memset(&bank,0xA5,sizeof(bank)); memset(oldData,0xDA,sizeof(oldData)); memset(newData,0x5B,sizeof(newData)); memset(heads,0xA5,sizeof(heads)); memset(records,0,sizeof(records));
        allocated=0; memset(nodes,0,sizeof(nodes)); g_CCharStringInstanceCount_013BD800=100; bank.OpenFlags=(seed%8)<<3;
        FableUiGraphicArray* arrays[]={&bank.Symbols,&bank.Checksums,&bank.RuntimeData,&bank.UpdateData};
        for(unsigned i=0;i<4;++i) { arrays[i]->Begin=oldData[i]; arrays[i]->End=oldData[i]; arrays[i]->Capacity=oldData[i]+((seed&(8<<i)) ? (i==2 ? 48 : 16) : 0); }
        unsigned count=mode ? seed%7 : (seed&8 ? seed%4 : 0);
        bank.Symbols.End=oldData[0]+count*4;
        for(i=0;i<count;++i) { unsigned index=(i+seed)%5; reinterpret_cast<FableUiStringValue*>(oldData[0])[i].Storage=records+index; ++records[index].owners; }
        for(i=0;i<8;++i) { records[i].unknown08=0x80000000u; records[i].flags0C=0xA5; }
        bank.SymbolIndices.Head=heads; bank.SymbolIndices.Count=seed&4 ? 3 : 0;
        bank.FilenameIndices.Head=heads+1; bank.FilenameIndices.Count=seed&2 ? 2 : 0;
        heads[0].Parent=heads+1; heads[1].Parent=heads;
        if(!mode) for(unsigned t=0;t<2;++t)
        {
            unsigned n=t ? (seed&2 ? 2 : 0) : (seed&4 ? 3 : 0);
            if(n) heads[t].Parent=&nodes[t*3].Links;
            for(unsigned j=0;j<n;++j)
            { unsigned index=t*3+j; nodes[index].Links.Right=j+1<n ? &nodes[index+1].Links : 0; nodes[index].Key.Storage=records+(seed+index)%8; ++nodes[index].Key.Storage->owners; }
        }
        printf("TRACE");
        if(mode)
        {
            unsigned first=(seed/7)%(count+1),last=first+(seed/49)%(count-first+1);
            FableUiStringValue* begin=reinterpret_cast<FableUiStringValue*>(oldData[0]);
            if(FableUiEraseStringRange(&bank.Symbols,0,begin+first,begin+last)!=begin+first) return 4;
        }
        else FableUiPrepareBankStorage(&bank,0,(seed/8)%8);
        printf(" BANK"); Dump(&bank,sizeof(bank)); printf(" OLD"); Dump(oldData,sizeof(oldData)); printf(" HEADS"); Dump(heads,sizeof(heads));
        printf(" NEW"); for(unsigned x=0;x<512;++x) printf("%02x",x<128 && x%12>=10 ? 0 : reinterpret_cast<unsigned char*>(newData)[x]);
        printf(" NODES"); Dump(nodes,sizeof(nodes));
        printf(" RECORDS"); for(i=0;i<sizeof(records);++i) printf("%02x",reinterpret_cast<unsigned char*>(records)[i]);
        printf(" COUNT%d END\n",g_CCharStringInstanceCount_013BD800);
    }
    return 0;
}
