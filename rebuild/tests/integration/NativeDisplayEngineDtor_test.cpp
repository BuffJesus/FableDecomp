#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display_engine_dtor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall UninitDisplayEngine(void*);
extern "C" void __fastcall DestroyDisplayEngine(void*);
extern "C" void* __fastcall DeleteDisplayEngine(void*,void*,unsigned);
extern "C" void __fastcall DeleteView(void*);
extern "C" unsigned char DisplayEngineTable[],BaseTable[],NonCopyTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
extern "C" void __fastcall DeleteNested(void*);
static void event(unsigned kind,unsigned a=0,unsigned b=0){
 unsigned h[]={kind,a,b};unsigned char copy[4096];memcpy(copy,arena,sizeof(copy));unsigned& v=*(unsigned*)copy;if(v==(unsigned)DisplayEngineTable)v=0x1231574;else if(v==(unsigned)BaseTable)v=0x1231710;else if(v==(unsigned)NonCopyTable)v=0x129a7c4;
 unsigned offsets[]={0x584,0x5c4,0x604,0x624};for(unsigned i=0;i<4;++i){unsigned& value=*(unsigned*)(copy+offsets[i]);if(value==(unsigned)&DeleteView)value=0x436ba0;else if(value==(unsigned)&DeleteNested)value=0x24009200;}
 fwrite(h,4,3,output);fwrite(copy,1,4096,output);
}
extern "C" void __fastcall ReleaseEngineBoundary(void* self){event(1,(unsigned)self);if(mode==1)word(0x2c)=(unsigned)(arena+0x440);}
extern "C" void* __fastcall GetManagerBoundary(){event(2);if(mode==2)word(0x2c)=(unsigned)(arena+0x440);return arena+0xa00;}
extern "C" void __fastcall RemoveEngineBoundary(void* self,void*,void* engine){event(3,(unsigned)self,(unsigned)engine);if(mode==3)word(0xe4)=(unsigned)(arena+0x620);}
extern "C" void __fastcall DeleteNested(void* self){event(4,(unsigned)self);if(mode==4){word(0xe4)=(unsigned)(arena+0x620);word(0x504)=(unsigned)(arena+0x5c0);}}
void operator delete(void* p){if(output)event(5,(unsigned)p);else free(p);}
extern "C" void UnusedDisplayEngineDtorBoundary(){exit(99);}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x800c)=(unsigned)&ReleaseEngineBoundary;
 unsigned p[2],cases=0;while(fread(p,4,2,input)==2){if(fread(arena,1,4096,input)!=4096)return 5;mode=p[1];word(0x584)=word(0x5c4)=(unsigned)&DeleteNested;word(0x604)=word(0x624)=(unsigned)&DeleteView;unsigned result=0;unsigned flags[]={0,1,2,0xffffffff};if(p[0]==0)UninitDisplayEngine(arena);else if(p[0]==1)DestroyDisplayEngine(arena);else result=(unsigned)DeleteDisplayEngine(arena,0,flags[p[0]-2]);event(6,p[0],result);++cases;}
 fclose(input);fclose(output);output=0;printf("NATIVE_DISPLAY_ENGINE_DTOR_PASS cases=%u\n",cases);return 0;
}
