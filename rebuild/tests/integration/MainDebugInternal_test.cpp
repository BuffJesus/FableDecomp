#include "fable_memory_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned long __security_cookie;
class Access:public CMemoryAllocator{public:using CMemoryAllocator::InternalAllocate;};
static unsigned char* const region=(unsigned char*)0x2004000;
static FILE* input;static unsigned long args[6],calls,queries;static unsigned char expected[0x8000],snapshot[0x8000];
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void boundary(unsigned long kind,unsigned long a,unsigned long b,unsigned long c){unsigned long event[4];if(calls>=4)exit(8);read(event,sizeof(event));read(snapshot,sizeof(snapshot));if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(region,snapshot,sizeof(snapshot))){printf("event %lu kind %lu failed\n",calls,kind);exit(9);}++calls;}
void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,bool debug){if(!debug)exit(10);boundary(1,(unsigned long)this,(unsigned long)size,category);if(args[4]){region[0x414]=7;__security_cookie=args[5]^0x12345678;}return region+0x6000;}
unsigned long CMemoryAllocator::GetAllocationSize(void* pointer){boundary(2,(unsigned long)this,(unsigned long)pointer,queries);unsigned long value=queries==0?0xffffffff:args[3];if(args[4])region[0x414]=queries==0?255:3;++queries;return value;}
extern "C" unsigned long __fastcall DebugCookieRecording(unsigned long cookie){boundary(3,cookie,0,0);if(cookie!=args[5])exit(11);return 0x200a000;}
int main(int argc,char** argv){if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;if(VirtualAlloc((void*)0x2000000,0x20000,0x3000,4)!=(void*)0x2000000)return 3;unsigned long cases;read(&cases,4);for(unsigned long i=0;i<cases;++i){read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));__security_cookie=args[5];calls=queries=0;void* result=((Access*)region)->InternalAllocate(args[0],args[1],(const char*)0xdeadbeef,-1,(const char*)0xbaadf00d);if(result!=region+0x6000||calls!=4||queries!=2||memcmp(region,expected,sizeof(expected))){printf("case %lu failed\n",i);return 4;}}if(fgetc(input)!=EOF)return 5;printf("MAIN_DEBUG_INTERNAL_NATIVE_OK cases=%lu\n",cases);return 0;}

