#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "allocate_engine_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall AllocateEngine();
static unsigned char* arena;static FILE* output;static unsigned fill,present,offset,mode;
static void event(unsigned kind,unsigned value){unsigned h[]={kind,value};fwrite(h,4,2,output);fwrite(arena,1,1024,output);}
void* __cdecl operator new(size_t size){if(!arena) return malloc(size);if(size!=0x178)exit(10);event(1,size);return present?arena+offset:0;}
extern "C" void* __fastcall ConstructEngine(void* self,void*){if(self!=arena+offset)exit(11);event(2,(unsigned)self);unsigned i;if(mode==1)for(i=0;i<0x178;++i)((unsigned char*)self)[i]=(unsigned char)(i*17+fill);else if(mode==2)for(i=0;i<1024;++i)arena[i]=(unsigned char)(i*3+fill);return self;}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[4],cases=0;while(fread(p,4,4,input)==4){if(fread(arena,1,1024,input)!=1024)return 5;fill=p[0];present=p[1];offset=p[2];mode=p[3];event(3,(unsigned)AllocateEngine());++cases;}
 fclose(input);fclose(output);printf("NATIVE_ALLOCATE_ENGINE_PASS cases=%u\n",cases);return 0;}
