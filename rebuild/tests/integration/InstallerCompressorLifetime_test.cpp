#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CCompressorZlib.h"
static unsigned live,baseCalls;
static void require(bool value){if(!value)abort();}
static void* __cdecl Allocate(void*,unsigned count,unsigned width){void* p=calloc(count,width);require(p!=0);++live;return p;}
static void __cdecl Release(void*,void* p){if(p){require(live!=0);--live;free(p);}}
// Vtable slots are required by the destructor but must never run in this fixture.
void CCompressorZlib::Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::SetQuality(float){abort();}
extern "C" void __fastcall BDtor(void*){++baseCalls;}
int main(){
 unsigned cases=0;
 for(unsigned cycle=0;cycle!=32;++cycle)for(unsigned compression=0;compression!=2;++compression){
  unsigned long storage[sizeof(CCompressorZlib)/4];memset(storage,0,sizeof(storage));
  CCompressorZlib* c=reinterpret_cast<CCompressorZlib*>(storage);
  c->Stream.zalloc=Allocate;c->Stream.zfree=Release;
  int status=compression?z_deflateInit_(&c->Stream,6,"1.2.1",sizeof(z_stream_s)):z_inflateInit_(&c->Stream,"1.2.1",sizeof(z_stream_s));
  require(status==0&&live!=0);c->ZlibInitialised=true;c->Compressing=compression!=0;
  c->Stream.next_in=(unsigned char*)0x1234;c->Stream.next_out=(unsigned char*)0x5678;c->Stream.avail_in=13;c->Stream.avail_out=17;
  c->CCompressorZlib::~CCompressorZlib();
  require(live==0&&!c->ZlibInitialised&&c->Stream.state==0&&baseCalls==cases+1);
  if(!compression)require(!c->Stream.next_in&&!c->Stream.next_out&&!c->Stream.avail_in&&!c->Stream.avail_out);
  ++cases;
 }
 printf("INSTALLER_COMPRESSOR_LIFETIME_OK cases=%u\n",cases);return 0;
}
