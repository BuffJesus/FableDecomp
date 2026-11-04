#include <stdio.h>
#include <stdlib.h>
#include "engine_uninitialise_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall UninitialiseEngine(void*);
extern "C" unsigned RenderManager,DisplayManager,PrimitiveShared,VideoOptions;
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned self,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){unsigned h[]={kind,self,a,b,c,d},globals[]={RenderManager,DisplayManager,PrimitiveShared,VideoOptions};fwrite(h,4,6,output);fwrite(globals,4,4,output);fwrite(arena,1,0x9000,output);}
static void swap(){if(mode&1)RenderManager=(unsigned)(RenderManager==(unsigned)(arena+0x1000)?arena+0x5000:arena+0x1000);}
extern "C" void __fastcall RemoveCallbackBoundary(void* self,void*,void* callback){if(self!=arena+0xb00||callback!=arena+12)exit(10);event(1,(unsigned)self,(unsigned)callback);swap();}
extern "C" long __stdcall StreamBoundary(void* self,unsigned stream,void* buffer,unsigned offset,unsigned stride){if((self!=arena+0x900&&self!=arena+0x940)||stream||buffer||offset||stride)exit(11);event(2,(unsigned)self,stream,(unsigned)buffer,offset,stride);swap();return 0x80004005;}
extern "C" long __stdcall IndicesBoundary(void* self,void* buffer){if(buffer)exit(12);event(3,(unsigned)self,0);swap();return 0x80004005;}
extern "C" long __stdcall TextureBoundary(void* self,unsigned stage,void* texture){if(stage>=4||texture)exit(13);event(4,(unsigned)self,stage,0);swap();return 0x80004005;}
extern "C" void* __fastcall DeleteBoundary(void* self,void*,unsigned flags){if(flags!=1)exit(14);unsigned kind=self>=arena+0x300&&self<arena+0x400?5:self>=arena+0x500&&self<arena+0x600?6:7;if(kind==7&&(self<arena+0x700||self>=arena+0x800))exit(15);event(kind,(unsigned)self,1);++((unsigned*)self)[1];if(mode&2){if(kind==5)word(0x16c)=word(0x168);else if(kind==6)word(0x150)=word(0x14c);else if(self==arena+0x700)word(0xa84)=0;}return (void*)0x80004005;}
extern "C" void __fastcall BaseUninitialiseBoundary(void* self){if(self!=arena)exit(16);event(8,(unsigned)self);arena[4]=0;arena[0x84]=0x7f;}
int main(int argc,char** argv){if(argc!=3)return 2;if(RenderManager||DisplayManager||PrimitiveShared||VideoOptions)return 17;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x40000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x30000+100*4)=(unsigned)&StreamBoundary;word(0x30000+104*4)=(unsigned)&IndicesBoundary;word(0x30000+65*4)=(unsigned)&TextureBoundary;word(0x32000)=(unsigned)&DeleteBoundary;
 unsigned p[5],cases=0;while(fread(p,4,5,input)==5){if(fread(arena,1,0x9000,input)!=0x9000)return 5;mode=p[0];RenderManager=p[1];DisplayManager=p[2];PrimitiveShared=p[3];VideoOptions=p[4];UninitialiseEngine(arena);event(9,(unsigned)arena);++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_UNINITIALISE_PASS cases=%u\n",cases);return 0;}
