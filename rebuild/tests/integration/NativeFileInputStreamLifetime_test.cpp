#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_data_input_stream.h"
#define CHECK(x) do {if(!(x)){fprintf(stderr,"INPUT_STREAM_FAILURE line=%d\n",__LINE__);exit(80);}}while(0)
static char emptyText[]="";char* g_FableEmptyNarrowString_0129AAF4=emptyText;
static void* allocation;static unsigned allocations,frees,reads,seeks;
void* operator new[](size_t n) {CHECK(!allocation);allocation=malloc(n);CHECK(allocation);++allocations;return allocation;}
void operator delete[](void* p) {CHECK(p && p==allocation);free(p);allocation=0;++frees;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
extern "C" void ColdBoundary(){fprintf(stderr,"INPUT_STREAM_FAILURE cold boundary\n");exit(80);}
struct File {void** vptr;long position,length;bool open;unsigned seed;};
static void __fastcall Seek(File* f,void*,long p){CHECK(p>=0 && p<=f->length);f->position=p;++seeks;}
static long __fastcall Position(File* f,void*){return f->position;}
static long __fastcall Length(File* f,void*){return f->length;}
static bool __fastcall IsOpen(File* f,void*){return f->open;}
static unsigned char Byte(File* f,unsigned p){return static_cast<unsigned char>(p*37+f->seed);}
static void __fastcall Read(File* f,void*,void* out,long n,void* process){CHECK(!process && n>=0 && f->position+n<=f->length);for(long i=0;i<n;++i)static_cast<unsigned char*>(out)[i]=Byte(f,f->position+i);f->position+=n;++reads;}
static void CheckRead(CFileDataInputStream& stream,File& file,unsigned n){unsigned char out[256];memset(out,0xcd,sizeof(out));unsigned position=stream.GetPosition();stream.Read(out+8,n);for(unsigned i=0;i<sizeof(out);++i)CHECK(out[i]==(i>=8 && i<8+n?Byte(&file,position+i-8):0xcd));CHECK(stream.GetPosition()==position+n);}
#include "NativeFileInputStreamColdAliases.h"
int main(){
 void* table[16];for(unsigned i=0;i<16;++i)table[i]=reinterpret_cast<void*>(ColdBoundary);
 table[3]=reinterpret_cast<void*>(Read);table[5]=reinterpret_cast<void*>(Seek);table[7]=reinterpret_cast<void*>(Position);table[9]=reinterpret_cast<void*>(Length);table[10]=reinterpret_cast<void*>(IsOpen);
 unsigned cases=0;unsigned buffers[]={4,8,16,16384};
 for(unsigned b=0;b<4;++b)for(unsigned start=0;start<3;++start)for(unsigned state=0;state<2;++state){
  File file={table,start*3,128,true,17+start};unsigned before=allocations,oldFree=frees;
  {CFileDataInputStream stream(reinterpret_cast<CAFile*>(&file),buffers[b]);CHECK(allocations==before+1 && stream.GetPosition()==start*3 && stream.GetLength()==128);
   unsigned beforeReads=reads;CheckRead(stream,file,1);CHECK(reads==beforeReads+1 && file.position==static_cast<long>(start*3+(buffers[b]<128-start*3?buffers[b]:128-start*3)));
   stream.SetPosition(start*3);CheckRead(stream,file,3);CHECK(reads==beforeReads+1);
   CheckRead(stream,file,b==3?5:buffers[b]+3);CHECK(reads==beforeReads+(b==3?1:2));
   stream.SetPosition(60);CheckRead(stream,file,33);CHECK(reads==beforeReads+(b==3?1:3));
   stream.SetPosition(17);CheckRead(stream,file,2);CHECK(reads==beforeReads+(b==3?1:4));
   file.open=state!=0;unsigned oldSeeks=seeks;long physical=file.position;stream.Close();CHECK(frees==oldFree+1 && !allocation && !stream.GetPosition() && !stream.GetLength());CHECK(file.position==(state?19:physical) && seeks==oldSeeks+(state?1:0));stream.Close();CHECK(frees==oldFree+1);
  }CHECK(frees==oldFree+1 && !allocation);++cases;
 }
 CHECK(allocations==cases && frees==cases);printf("NATIVE_INPUT_STREAM_PASS cases=%u allocations=%u frees=%u reads=%u seeks=%u\n",cases,allocations,frees,reads,seeks);return 0;
}
