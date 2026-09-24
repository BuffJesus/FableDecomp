#include "fable_ui_bank_stream.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static unsigned char storage[52],buffer[128],destination[128];
static CFileDataInputStream* stream=reinterpret_cast<CFileDataInputStream*>(storage+8);
static unsigned seed;
struct File { FableUiStreamFileVtable* Vtable; unsigned Position,Id; };
static File files[2];
static unsigned __fastcall Position(CFileDataInputStream* p,void*)
{ if(seed&256) p->BufferSize=2; return FableUiStreamPrefix(p)->StreamPos; }
static void __fastcall SetPosition(void* p,void*,unsigned pos)
{ File* f=static_cast<File*>(p); printf(" SET%u:%u",f->Id,pos); f->Position=pos; if(seed&32) stream->File=reinterpret_cast<CAFile*>(files+1); }
static void __fastcall Read(void* p,void*,void* target,long n,bool flag)
{
    File* f=static_cast<File*>(p); unsigned char* out=static_cast<unsigned char*>(target);
    unsigned id=out>=buffer && out<buffer+128 ? 1000+static_cast<unsigned>(out-buffer) : 2000+static_cast<unsigned>(out-destination);
    printf(" READ%u:%u:%ld:%u",f->Id,id,n,flag); if(n<0 || n>64) abort();
    for(long i=0;i<n;++i) out[i]=static_cast<unsigned char>((f->Position+i)*13+f->Id);
    f->Position+=n;
    if(seed&16) stream->Buffer=buffer+64;
    if(seed&8) FableUiStreamPrefix(stream)->StreamPos=0x1234;
}
static void Dump()
{
    printf(" STATE");
    for(unsigned i=0;i<52;i+=4)
    {
        unsigned v; memcpy(&v,storage+i,4);
        if(i==8) v=0x70000300;
        if((i==20 || i==36) && v) v=0x66000000+v-reinterpret_cast<unsigned>(buffer);
        if(i==32) v=static_cast<File*>(static_cast<void*>(stream->File))->Id;
        printf(":%08x",v);
    }
    printf(" FILES:%u:%u BUFFER",files[0].Position,files[1].Position);
    for(unsigned j=0;j<128;++j) printf("%02x",buffer[j]);
    printf(" OUT"); for(j=0;j<128;++j) printf("%02x",destination[j]);
}
int main()
{
    FableUiBankStreamVtable table={}; table.GetPosition=Position; table.GetSource=FableUiGetBankStreamSource;
    table.UseBuffer=FableUiBankStreamUseBuffer; table.ReadDirect=FableUiReadBankStreamDirect;
    FableUiStreamFileVtable ft={}; ft.SetPosition=SetPosition; ft.Unrecovered00[3]=reinterpret_cast<void*>(Read);
    const unsigned positions[]={0,17,0x7ffffff0,0xfffffff0}; const long lengths[]={0,1,3,7,8,15,16,31};
    for(unsigned mode=0;mode<4;++mode) for(seed=0;seed<512;++seed)
    {
        memset(storage,0xA5,sizeof(storage)); memset(buffer,0xCD,sizeof(buffer)); memset(destination,0xEE,sizeof(destination));
        for(unsigned i=0;i<16;++i) buffer[i+8]=static_cast<unsigned char>(0xA0+i);
        for(i=0;i<2;++i) { files[i].Vtable=&ft; files[i].Position=77+i; files[i].Id=i+1; }
        CDataInputStream* prefix=FableUiStreamPrefix(stream); prefix->__vftable=&table;
        prefix->StreamPos=positions[(seed>>6)%4]; prefix->StreamSize=prefix->StreamPos+256;
        prefix->CurrentSourcePtr=buffer+8; prefix->SourceChunkStreamPos=prefix->StreamPos-2;
        prefix->SourceChunkBytesRemaining=seed%4;
        stream->File=reinterpret_cast<CAFile*>(files); stream->Buffer=buffer+8; stream->BufferSize=4<<((seed>>2)%4);
        long n=lengths[(seed>>3)%8]; printf("TRACE");
        if(mode==0) FableUiReadBankStreamSlow(stream,0,destination+8,n+prefix->SourceChunkBytesRemaining);
        if(mode==1) { void* out=0; long count=-1; FableUiGetBankStreamSource(stream,0,&out,&count); printf(" SOURCE%u:%ld",static_cast<unsigned>(static_cast<unsigned char*>(out)-buffer),count); }
        if(mode==2) FableUiReadBankStreamDirect(stream,0,destination+8,n);
        if(mode==3) printf(" USE%u",FableUiBankStreamUseBuffer(stream,0,seed&256 ? -1 : n));
        Dump(); printf(" END\n");
    }
    return 0;
}
