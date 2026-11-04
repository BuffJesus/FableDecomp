#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "iengine_lifetime_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned char __fastcall InitialiseEngine(void*,void*,const void*);
extern "C" void __fastcall ReleaseEngine(void*);
extern "C" void __fastcall SetSeaBody(void*,void*,unsigned);
extern "C" void* __fastcall ScalarDeleteEngine(void*,void*,unsigned);
extern "C" unsigned char IEngineTable[];
static unsigned char* arena;static FILE* output;static unsigned mode,parameter;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned self,unsigned a=0,unsigned b=0){unsigned h[]={kind,self,a,b};fwrite(h,4,4,output);unsigned p=word(0);if(p==(unsigned)IEngineTable)word(0)=0x12a0d24;fwrite(arena,1,512,output);word(0)=p;}
extern "C" unsigned char __fastcall InitBoundary(void* self){if(self!=arena)exit(10);event(1,(unsigned)self);arena[4]=(unsigned char)parameter;if(mode&1)word(0x40)=0x87654321;return (unsigned char)parameter;}
extern "C" void* __fastcall VirtualDeleteBoundary(void* self,void*,unsigned flags){if(self!=arena||flags!=1)exit(11);event(2,(unsigned)self,flags);if(mode&1)word(8)=0xabcdef01;if(mode&2)word(0)=0x12345678;return 0;}
extern "C" void __fastcall StringDtorBoundary(unsigned* self){unsigned data=*self;event(3,1,data);*self=0;if(data&&(mode&1))*(unsigned*)data^=0x12345678;if(mode&2)word(0x40)=0x98765432;}
extern "C" void __fastcall BaseDtorBoundary(void* self){if(self!=arena)exit(12);event(4,(unsigned)self);word(0)=0x129a7cc;if(mode&1)word(8)=0x87654321;if(mode&2)word(0x40)=0x76543210;}
void __cdecl operator delete(void* self){if(!arena){free(self);return;}if(self!=arena)exit(13);event(5,(unsigned)self);if(mode&1)word(0x44)=0x10293847;}
extern "C" void UnusedIEngineLifetimeBoundary(){exit(99);}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8000)=(unsigned)&VirtualDeleteBoundary;
 unsigned p[3],cases=0;while(fread(p,4,3,input)==3){if(fread(arena,1,512,input)!=512)return 5;unsigned op=p[0];parameter=p[1];mode=p[2];if(word(0)==0x12a0d24)word(0)=(unsigned)IEngineTable;unsigned value=0;
 if(op==0)value=InitialiseEngine(arena,0,arena+0x100);else if(op==1)ReleaseEngine(parameter?arena:0);else if(op==2){unsigned data[]={0,(unsigned)(arena+0x100),(unsigned)(arena+0x180),(unsigned)(arena+0x1f0)};SetSeaBody(arena,0,data[parameter]);}else{unsigned flags[]={0,1,2,0xffffffff};value=(unsigned)ScalarDeleteEngine(arena,0,flags[parameter]);}event(6,op,value);++cases;}
 fclose(input);fclose(output);printf("NATIVE_IENGINE_LIFETIME_PASS cases=%u\n",cases);return 0;}
