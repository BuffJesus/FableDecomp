#include "test_support/compressor_virtual_stubs.h"
#include <stdio.h>
#include <string.h>
static int de,ie,base,frees,bad;static CCompressorZlib* current;static unsigned char storage[8];
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){++base;if(current->ZlibInitialised)bad=1;}
extern "C" void __cdecl free(void* p){++frees;if(p!=storage)bad=2;}
int __cdecl z_deflateEnd(z_stream_s* s){++de;if(s!=&current->Stream)bad=3;return -1;}
int __cdecl z_inflateEnd(z_stream_s* s){++ie;if(s!=&current->Stream)bad=4;return -1;}
int main(){ de=ie=base=frees=bad=0;
 double raw[sizeof(CCompressorZlib)/8];
 for(int mode=0;mode<3;++mode){memset(raw,0,sizeof(raw));current=reinterpret_cast<CCompressorZlib*>(raw);
 current->ZlibInitialised=mode!=0;current->Compressing=mode==1;
 current->OwnWorkspace._M_start=mode?storage:0;current->OwnWorkspace._M_finish=mode?storage+8:0;
 current->Stream.next_in=storage;current->Stream.next_out=storage+1;current->Stream.avail_in=8;current->Stream.avail_out=7;
 current->CCompressorZlib::~CCompressorZlib();
 if(mode==2 && (current->Stream.next_in || current->Stream.next_out || current->Stream.avail_in || current->Stream.avail_out))return 1;
 if(mode!=2 && (current->Stream.next_in!=storage || current->Stream.avail_in!=8))return 2;
 }
 if(de!=1 || ie!=1 || base!=3 || frees!=2 || bad){printf("de=%d ie=%d base=%d frees=%d bad=%d\n",de,ie,base,frees,bad);return 3;}
 puts("DTOR_PASS");return 0;
}