#include "engine/CEngineLandscapeMemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "landscape_pool_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Toggle(void*,void*,unsigned long);
extern "C" void __fastcall Dispose(void*);
#pragma comment(linker,"/alternatename:@Toggle@12=?EnablePoolAllocation@CEngineLandscapeMemoryManager@@QAEX_N@Z")
#pragma comment(linker,"/alternatename:@Dispose@4=?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ")
static unsigned char* arena;static FILE* output;static unsigned mode;
static void record(unsigned kind){if(fwrite(&kind,4,1,output)!=1||fwrite(arena,1,256,output)!=256)exit(9);}
void CEngineLandscapeMemoryManager::FreeDisposalList(){if(this!=(CEngineLandscapeMemoryManager*)arena)exit(10);record(1);if(mode==1)memset(arena,0xac,256);if(mode==2)arena[0x39]^=0xff;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+14)=(unsigned)Dispose-(unsigned)(code+18);
 typedef void (__fastcall *Call)(void*,void*,unsigned long);Call run=argv[3][0]=='r'?(Call)code:Toggle;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,256,in)!=256)return 5;mode=p[0];run(arena,0,p[1]);record(2);run(arena,0,p[2]);record(3);++cases;}
 fclose(in);fclose(output);printf("NATIVE_LANDSCAPE_POOL_PASS cases=%u\n",cases);return 0;
}
