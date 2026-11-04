#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"
extern CCharString SeaStaticMapBankFileName,WaterStaticMapBankFileName,CommonStaticMapHeaderEntryName;
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
#pragma comment(linker,"/alternatename:??0CCharString@@QAE@PBDJ@Z=@Construct@16")
#pragma comment(linker,"/alternatename:??1CCharString@@QAE@XZ=@Destroy@4")
static unsigned char* arena;static unsigned char events[32*1044];static unsigned used;static const char* outputPath;
static unsigned id(void* self){if(self==&SeaStaticMapBankFileName)return 0;if(self==&WaterStaticMapBankFileName)return 1;if(self==&CommonStaticMapHeaderEntryName)return 2;exit(20);return 0;}
static void record(unsigned kind,unsigned arg){
 if(used+1044>sizeof(events))exit(21);unsigned h[5]={kind,arg,*(unsigned*)&SeaStaticMapBankFileName,*(unsigned*)&WaterStaticMapBankFileName,*(unsigned*)&CommonStaticMapHeaderEntryName};
 memcpy(events+used,h,20);memcpy(events+used+20,arena,1024);used+=1044;
}
extern "C" void* __fastcall Construct(void* self,void*,const char* text,long length){
 static const char* names[]={"__ENGINE_SEA_STATIC_MAP_BANK_FILE__","__ENGINE_WATER_STATIC_MAP_BANK_FILE__","__STATIC_MAP_COMMON_HEADER__"};
 unsigned n=id(self);if(length!=-1||strcmp(text,names[n]))exit(22);record(1,n);
 *(unsigned*)self=(unsigned)(arena+0x100+32*n);*(unsigned*)(arena+0x100+32*n)=n+1;record(2,n);return self;
}
extern "C" void __fastcall Destroy(void* self){unsigned n=id(self);record(3,n);*(unsigned*)(arena+0x100+32*n)=0;*(unsigned*)self=0;record(4,n);}
static void finish(){record(9,0);FILE* f=fopen(outputPath,"wb");if(!f||fwrite(events,1,used,f)!=used)exit(23);fclose(f);}
#pragma init_seg(lib)
struct Observer{Observer(){arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)exit(24);atexit(finish);}};
static Observer observer;
int main(int argc,char** argv){if(argc!=2)return 2;outputPath=argv[1];record(5,0);printf("NATIVE_MAP_NAME_GLOBALS_PASS\n");return 0;}
