#include "UiWideStringFixture.h"
#include "fable_ui_bank_decode.h"
fable_i32 g_CCharStringInstanceCount_013BD800;
static unsigned char temporary[1024],payload[512],refill[256],storage[52];
static unsigned arraySize,arrayCount,seed,mode,filePosition;
static CFileDataInputStream* stream=reinterpret_cast<CFileDataInputStream*>(storage+8);
static FableUiStreamFileVtable fileTable;
static FableUiStreamFileVtable* file=&fileTable;
void* __cdecl FableUiAllocateArchiveArray(unsigned n)
{ printf(" TA%u",n); if(mode==1 && (seed&128) && seed%32==1) { printf(" FAIL"); return 0; } if(n>1000) abort(); arraySize=n; ++arrayCount; return temporary; }
void __cdecl FableUiFreeArchiveArray(void* p) { printf(" TF%u",p==temporary); }
void* __cdecl FableUiAllocateStringBuffer(unsigned n) { return FableUiAllocateWideBuffer(n); }
void __cdecl FableUiFreeStringBuffer(void* p) { FableUiFreeWideBuffer(p); }
static void __fastcall Seek(void*,void*,unsigned n) { printf(" SEEK%u",n); filePosition=n; }
static void __fastcall Read(void*,void*,void* out,long n,bool flag)
{ printf(" READ%ld:%u",n,flag); if(n<0 || filePosition+static_cast<unsigned>(n)>512) abort(); memcpy(out,payload+filePosition,n); filePosition+=n; }
static unsigned Normalize(unsigned v)
{
    if(v>=reinterpret_cast<unsigned>(temporary) && v<=reinterpret_cast<unsigned>(temporary+1024)) return 0x61000000+v-reinterpret_cast<unsigned>(temporary);
    if(v>=reinterpret_cast<unsigned>(payload) && v<=reinterpret_cast<unsigned>(payload+512)) return 0x62000000+v-reinterpret_cast<unsigned>(payload);
    if(v>=reinterpret_cast<unsigned>(refill) && v<=reinterpret_cast<unsigned>(refill+256)) return 0x63000000+v-reinterpret_cast<unsigned>(refill);
    return v;
}
static void DumpArray(FableUiGraphicArray* a)
{ printf(" ARRAY:%08x:%08x:%08x",Normalize(reinterpret_cast<unsigned>(a->Begin)),Normalize(reinterpret_cast<unsigned>(a->End)),Normalize(reinterpret_cast<unsigned>(a->Capacity))); }
int main()
{
    FableUiBankStreamVtable table={}; table.GetPosition=FableUiBankStreamPosition; table.GetSource=FableUiGetBankStreamSource; table.UseBuffer=FableUiBankStreamUseBuffer; table.ReadDirect=FableUiReadBankStreamDirect;
    fileTable.SetPosition=Seek; fileTable.Unrecovered00[3]=reinterpret_cast<void*>(Read);
    const unsigned lengths[]={0,1,3,7,15,31,63,127},available[]={0,2,4,256},patterns[]={0,0xA5,0xFF,17};
    for(mode=0;mode<3;++mode) for(seed=0;seed<(mode==2 ? 1024u : 256u);++seed)
    {
        memset(temporary,0xCD,sizeof(temporary)); arraySize=arrayCount=0; printf("TRACE");
        if(mode<2)
        {
            FableUiGraphicArray a; memset(&a,patterns[(seed>>5)%4],sizeof(a));
            FableUiGraphicArray* result=mode ? FableUiConstructArchivePairs(&a,0,seed%32) : FableUiConstructArchiveBytes(&a,0,seed%32);
            if(result!=&a) return 4; DumpArray(&a);
        }
        else
        {
            memset(records,0xA5,sizeof(records)); memset(buffers,0xCD,sizeof(buffers)); memset(values,0,sizeof(values));
            recordCount=bufferCount=attempt=0; failAt=seed&512 ? 1 : 0; g_CWideStringInstanceCount_013BCA20=123; g_CCharStringInstanceCount_013BD800=100;
            unsigned length=lengths[seed%8],kind=(seed>>7)%4; memset(payload,0xA7,sizeof(payload)); memcpy(payload,&length,4);
            for(unsigned i=0;i<length;++i) payload[4+i]=static_cast<unsigned char>(kind==3 ? 0x80+i%127 : 33+i%80);
            if(length && kind==1) payload[4]=0; if(length && kind==2) payload[4+length/2]=0;
            memset(storage,0xA5,sizeof(storage)); memset(refill,0xDA,sizeof(refill)); filePosition=0;
            CDataInputStream* prefix=FableUiStreamPrefix(stream); prefix->__vftable=&table; prefix->StreamPos=0; prefix->StreamSize=512;
            prefix->CurrentSourcePtr=payload; prefix->SourceChunkStreamPos=0; prefix->SourceChunkBytesRemaining=available[(seed>>3)%4];
            stream->File=reinterpret_cast<CAFile*>(&file); stream->Buffer=refill; stream->BufferSize=4<<((seed>>5)%4);
            if(FableUiReadArchiveString(stream,0,reinterpret_cast<FableUiStringValue*>(values))!=reinterpret_cast<FableUiStringValue*>(values)) return 5;
            Snapshot(); printf(" N%d STREAM",g_CCharStringInstanceCount_013BD800);
            for(i=0;i<52;i+=4) { unsigned v; memcpy(&v,storage+i,4); if(i==8) v=0x70000000; else if(i==32) v=0x70000100; else v=Normalize(v); printf(":%08x",v); }
            printf(" FILE%u REFILL",filePosition); for(i=0;i<256;++i) printf("%02x",refill[i]);
            FableUiDestroyBankName(reinterpret_cast<FableUiStringValue*>(values),0); Snapshot(); printf(" N%d",g_CCharStringInstanceCount_013BD800);
        }
        printf(" TEMP%u:",arrayCount); for(unsigned j=0;j<(arrayCount ? arraySize+4 : 4);++j) printf("%02x",temporary[j]);
        printf(" END\n");
    }
    return 0;
}
