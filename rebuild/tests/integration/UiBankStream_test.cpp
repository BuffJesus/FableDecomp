#include "fable_ui_bank_stream.h"
#include <stdio.h>
#include <string.h>
void* FableUiBaseVtable=reinterpret_cast<void*>(0x70000100);
void* FableUiDataInputStreamVtable=reinterpret_cast<void*>(0x70000301);
void* FableUiDestroyedBaseVtable=reinterpret_cast<void*>(0x70000302);
FableUiBankStreamVtable FableUiFileInputStreamVtable={0,0,FableUiBankStreamPosition};
static unsigned char storage[52],buffer[256];
static CFileDataInputStream* stream=reinterpret_cast<CFileDataInputStream*>(storage+8);
static unsigned seed;
struct File { FableUiStreamFileVtable* Vtable; unsigned Id; };
static File files[2];
static unsigned FileId(void* p) { return static_cast<File*>(p)->Id; }
static unsigned __fastcall Length(void* p,void*) { printf(" LEN%u",FileId(p)); if(seed&8) stream->File=reinterpret_cast<CAFile*>(&files[1]); return seed*0x01010101u; }
static unsigned __fastcall Position(void* p,void*) { printf(" POS%u",FileId(p)); return seed&2 ? seed*13 : 0; }
static bool __fastcall Seekable(void* p,void*) { printf(" SEEKABLE%u",FileId(p)); if(seed&16) stream->File=reinterpret_cast<CAFile*>(&files[1]); return (seed&4)!=0; }
static void __fastcall SetPosition(void* p,void*,unsigned n) { printf(" SET%u:%u",FileId(p),n); }
void* __cdecl FableUiAllocateStringBuffer(unsigned n) { printf(" ALLOC%u",n); return seed&32 ? 0 : buffer; }
void __cdecl FableUiFreeStringBuffer(void* p) { printf(" FREE%u",p==buffer); if(seed&8) stream->File=reinterpret_cast<CAFile*>(&files[1]); }
static unsigned Normalize(unsigned v)
{
    if(v==reinterpret_cast<unsigned>(&FableUiFileInputStreamVtable)) return 0x70000300;
    if(v==reinterpret_cast<unsigned>(&files[0])) return 0x65000000;
    if(v==reinterpret_cast<unsigned>(&files[1])) return 0x65000008;
    if(v==reinterpret_cast<unsigned>(buffer)) return 0x66000000;
    return v;
}
static void Dump()
{
    printf(" DATA");
    for(unsigned i=0;i<52;i+=4)
    {
        unsigned v; memcpy(&v,storage+i,4);
        if(i==20 && v) v=0x66000000+v-reinterpret_cast<unsigned>(buffer);
        else v=Normalize(v);
        printf(":%08x",v);
    }
}
int main()
{
    FableUiStreamFileVtable table={}; table.SetPosition=SetPosition; table.GetPosition=Position; table.GetLength=Length; table.CanSeek=Seekable;
    files[0].Vtable=files[1].Vtable=&table; files[0].Id=1; files[1].Id=2;
    for(unsigned mode=0;mode<4;++mode) for(seed=0;seed<256;++seed)
    {
        memset(storage,0xA5,sizeof(storage)); CDataInputStream* base=FableUiStreamPrefix(stream);
        printf("TRACE");
        if(mode==0)
        {
            unsigned sizes[4]={0,1,64,0xFFFFFFFF};
            if(FableUiConstructBankStream(stream,0,&files[0],sizes[seed%4])!=stream) return 1;
        }
        else
        {
            stream->__vftable=&FableUiFileInputStreamVtable;
            base->StreamPos=(seed%16)*4; base->StreamSize=seed*0x1010101u;
            base->CurrentSourcePtr=seed&1 ? 0 : buffer+128;
            base->SourceChunkStreamPos=seed&64 ? 0 : base->StreamPos-4;
            base->SourceChunkBytesRemaining=seed&128 ? -4 : 16;
            stream->File=seed&2 ? 0 : reinterpret_cast<CAFile*>(&files[0]); stream->Buffer=seed&1 ? 0 : buffer; stream->BufferSize=12345;
            if(mode==1) FableUiCloseBankStream(stream,0);
            if(mode==2) FableUiDestroyBankStream(stream,0);
            if(mode==3)
            {
                unsigned positions[8]={base->StreamPos,base->StreamPos+16,base->StreamPos+17,base->SourceChunkStreamPos,base->StreamPos-1,0xFFFFFFFF,0,seed*0x1010101u};
                FableUiSeekBankStream(stream,0,positions[(seed>>1)%8]);
            }
        }
        Dump(); printf(" END\n");
    }
    return 0;
}
