#include "fable_ui_bank_entries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
fable_i32 g_CCharStringInstanceCount_013BD800;
static FableUiBankFileView bank;
static unsigned char streamStorage[128],payload[4096],arrays[64][256],records[32][32],buffers[64][256];
static unsigned ac,rc,bc,seed,used,entryCount;
static CFileDataInputStream* stream=reinterpret_cast<CFileDataInputStream*>(streamStorage);
static void* bankTable[16];
static FableUiBankStreamVtable streamTable;
static void Hex(const void* p,unsigned n) { for(unsigned i=0;i<n;++i) printf("%02x",static_cast<const unsigned char*>(p)[i]); }
static unsigned Id(void* p,unsigned char* base,unsigned stride) { return p ? (static_cast<unsigned char*>(p)-base)/stride+1 : 0; }
void* __cdecl FableUiAllocateArchiveArray(unsigned n) { printf(" A%u",n); if(n>256 || ac>=64) abort(); return arrays[ac++]; }
void __cdecl FableUiFreeArchiveArray(void* p) { printf(" AF%u",Id(p,arrays[0],256)); }
void* __cdecl FableUiAllocateStringRecord(unsigned n) { printf(" R%u",n); if(n>32 || rc>=32) abort(); return records[rc++]; }
void __cdecl FableUiFreeStringRecord(void* p) { printf(" RF%u",Id(p,records[0],32)); }
void* __cdecl FableUiAllocateStringBuffer(unsigned n) { printf(" B%u",n); if(n>256 || bc>=64) abort(); return buffers[bc++]; }
void __cdecl FableUiFreeStringBuffer(void* p) { printf(" BF%u",Id(p,buffers[0],256)); }
static void __fastcall Table(FableUiBankFileView*,void*,unsigned size,FableUiGraphicArray* pairs)
{ printf(" TABLE%u:",size); Hex(pairs->Begin,static_cast<unsigned char*>(pairs->End)-static_cast<unsigned char*>(pairs->Begin)); }
static void __fastcall Entry(FableUiBankFileView*,void*,unsigned type,unsigned index,FableUiGraphicArray* bytes)
{
    printf(" ENTRY%u:%u:",type,index); Hex(bytes->Begin,static_cast<unsigned char*>(bytes->End)-static_cast<unsigned char*>(bytes->Begin));
    FableUiStreamPrefix(stream)->StreamPos+=seed%7;
}
static void __fastcall Finish(FableUiBankFileView* self,void*) { printf(" FINISH%u",self->FileValid); }
static void __fastcall Seek(CFileDataInputStream* self,void*,unsigned pos)
{ printf(" SEEK%u:%u",FableUiStreamPrefix(self)->StreamPos,pos); FableUiStreamPrefix(self)->StreamPos=pos; FableUiStreamPrefix(self)->CurrentSourcePtr=payload+pos; FableUiStreamPrefix(self)->SourceChunkBytesRemaining=used-pos; }
static bool __fastcall UseBuffer(CFileDataInputStream*,void*,long) { return false; }
static void __fastcall Direct(CFileDataInputStream* self,void*,void* target,long n)
{ unsigned pos=FableUiStreamPrefix(self)->StreamPos; printf(" READ%u:%u",pos,n); if(pos+n>used) abort(); memcpy(target,payload+pos,n); }
void __fastcall FableUiRegisterBankEntry(FableUiBankFileView* self,void*,unsigned index,const FableUiStringValue* symbol,const FableUiStringValue* file)
{ printf(" INDEX%u:%s:%s",index,FableUiStringText(symbol),FableUiStringText(file)); if(seed&128) self->SymbolCRCFlag=true; }
void __fastcall FableUiSortBankChecksums(void* begin,void* end,unsigned char mode) { printf(" SORT%u:%u:%u",begin==0,end==0,mode); }
void __fastcall FableUiCompactBankChecksums(FableUiGraphicArray*,void*) { printf(" COMPACT%u",bank.SymbolCRCFlag); if(seed&256) bank.ReadOnly=!bank.ReadOnly; }
void __fastcall FableUiPackBankRuntime(FableUiBankFileView* self,void*) { printf(" PACK%u",self->Size); }
static void Word(unsigned v) { memcpy(payload+used,&v,4); used+=4; }
static void Text(unsigned n,unsigned salt) { Word(n); for(unsigned i=0;i<n;++i) payload[used++]=static_cast<unsigned char>('A'+(seed+salt+i)%26); }
int main()
{
    bankTable[10]=reinterpret_cast<void*>(Table); bankTable[12]=reinterpret_cast<void*>(Entry); bankTable[15]=reinterpret_cast<void*>(Finish);
    memset(&streamTable,0,sizeof(streamTable)); streamTable.Unrecovered00[1]=reinterpret_cast<void*>(Seek); streamTable.GetPosition=FableUiBankStreamPosition; streamTable.UseBuffer=UseBuffer; streamTable.ReadDirect=Direct;
    for(seed=0;seed<512;++seed)
    {
        memset(&bank,0,sizeof(bank)); memset(streamStorage,0,sizeof(streamStorage)); memset(arrays,0xCD,sizeof(arrays)); memset(records,0xCD,sizeof(records)); memset(buffers,0xCD,sizeof(buffers));
        ac=rc=bc=used=0; g_CCharStringInstanceCount_013BD800=0; entryCount=(seed/8)%4;
        bank.Vtable=bankTable; bank.OpenFlags=(seed%8)*8; bank.ReadOnly=(seed&32)!=0; bank.SymbolCRCFlag=(seed&64)!=0; bank.UnrecoveredDC=seed%3;
        unsigned pairCount=(seed/4)%3; Word(pairCount);
        for(unsigned p=0;p<pairCount;++p) { Word(seed+p); Word(seed*17+p); }
        for(unsigned e=0;e<entryCount;++e)
        {
            Word(0xDEAD0000+e); Word(entryCount-e); Word(0x12340000+seed+e); Word(seed*11+e); Word(seed*23+e); Word(seed*37+e);
            Text((seed+e)%5,e); Word(seed*41+e); unsigned aliases=(seed+e)%3; Word(aliases);
            for(unsigned a=0;a<aliases;++a) Text((seed+a)%5,a+e);
            unsigned blob=(seed+e)%9; Word(blob); for(unsigned b=0;b<blob;++b) payload[used++]=static_cast<unsigned char>(seed+b+e);
        }
        CDataInputStream* prefix=FableUiStreamPrefix(stream); prefix->__vftable=&streamTable; prefix->StreamSize=used; prefix->CurrentSourcePtr=payload; prefix->SourceChunkBytesRemaining=seed%3==0 ? used : seed%3==1 ? 0 : 3;
        printf("TRACE"); bool result=FableUiReadBankEntries(&bank,0,stream,entryCount+1,entryCount);
        printf(" RESULT%u:%u:%u:%u:%u:%u:%d",result,bank.Size,bank.FileValid,bank.SymbolCRCFlag,bank.ReadOnly,prefix->StreamPos,g_CCharStringInstanceCount_013BD800);
        for(unsigned i=0;i<bank.Size;++i)
        {
            FableUiBankRuntimeEntry* v=static_cast<FableUiBankRuntimeEntry*>(bank.RuntimeData.Begin)+i;
            printf(" DATA%u:%u:%u:%u",v->Offset,v->Size,v->Type,v->Valid);
            if(bank.OpenFlags&8) printf(" SYMBOL:%s",FableUiStringText(static_cast<FableUiStringValue*>(bank.Symbols.Begin)+i));
            if(bank.OpenFlags&16) printf(" CRC%u",static_cast<unsigned*>(bank.Checksums.Begin)[i]);
            if(bank.OpenFlags&32)
            {
                FableUiBankUpdateEntry* u=static_cast<FableUiBankUpdateEntry**>(bank.UpdateData.Begin)[i];
                if(!u) printf(" UPDATE0"); else
                {
                    printf(" UPDATE%u:%u:%u:%u:",u->Metadata,u->Length,u->Unrecovered14,u->Unrecovered15); Hex(u->Bytes,u->Length);
                    for(unsigned a=0;a<static_cast<unsigned>(u->Aliases.Count);++a) printf(" ALIAS:%s",FableUiStringText(u->Aliases.Data+a));
                }
            }
        }
        printf(" END\n");
    }
    return 0;
}
