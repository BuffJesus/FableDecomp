#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display_engine_init_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall InitialiseDisplayEngine(void*);
extern "C" void __fastcall DeleteView(void*);
static unsigned char* arena;static FILE* output;static unsigned mode,refs,failure;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
extern "C" void __fastcall DeleteNested(void*);
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){
 unsigned h[]={kind,a,b,c,d};unsigned char copy[4096];memcpy(copy,arena,sizeof(copy));
 unsigned offsets[]={0x584,0x5c4,0x604,0x624,0x944};for(unsigned i=0;i<5;++i){unsigned& value=*(unsigned*)(copy+offsets[i]);if(value==(unsigned)&DeleteView)value=0x436ba0;else if(value==(unsigned)&DeleteNested)value=0x24009100;}
 fwrite(h,4,5,output);fwrite(copy,1,4096,output);
}
extern "C" void* __fastcall StringCtorBoundary(void* self,void*,const char* text,long length){
 unsigned label=!strcmp(text,"Init Display Engine: Init Graphics")?1:!strcmp(text,"Init Display Engine: Init Engine")?2:0;if(!label||length!=-1)exit(10);event(1,label,length);*(unsigned*)self=(unsigned)(arena+0xa00+label*16);if(mode==1)word(0x68)=0x11223344;return self;
}
extern "C" void __fastcall StringDtorBoundary(void* self){event(3,*(unsigned*)self);*(unsigned*)self=0;}
extern "C" void __fastcall ProgressBoundary(const unsigned* self,float fraction,bool force,bool debug){unsigned label=(*self-(unsigned)arena-0xa00)/16;event(2,label,*(unsigned*)&fraction,force,debug);if(mode==2)word(0)=(unsigned)(arena+0x8020);if(mode==3&&label==2)word(0x68)=0x87654321;}
static bool engineInit(void* self,unsigned slot){event(4,(unsigned)self,slot);if(mode==1){arena[0x90]=0;word(0x98)=word(0x9c)=0xdeadbeef;}return (refs&1)!=0;}
extern "C" bool __fastcall EngineInitOne(void* self){return engineInit(self,1);}
extern "C" bool __fastcall EngineInitTwo(void* self){return engineInit(self,2);}
void* operator new(size_t n){if(!output)return malloc(n);if(n!=8&&n!=12)exit(11);event(5,n);if(mode==1){arena[0x90]=0;word(0x98)=0x76543210;}if(mode==2&&n==8)word(0xe4)=(unsigned)(arena+0x620);if(mode==2&&n==12)word(0xe0)=(unsigned)(arena+0x980);return failure==n?0:arena+(n==8?0x900:0x940);}
void operator delete(void* p){if(output)event(7,(unsigned)p);else free(p);}
extern "C" void __fastcall DeleteNested(void* self){event(6,(unsigned)self);if(mode==3)word(0x504)=(unsigned)(arena+0x5c0);}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8004)=(unsigned)&EngineInitOne;word(0x8024)=(unsigned)&EngineInitTwo;
 unsigned p[3],cases=0;while(fread(p,4,3,input)==3){if(fread(arena,1,4096,input)!=4096)return 5;refs=p[0];failure=p[1];mode=p[2];word(0x584)=word(0x5c4)=(unsigned)&DeleteNested;word(0x604)=word(0x624)=(unsigned)&DeleteView;InitialiseDisplayEngine(arena);event(8);DeleteView(0);event(9);++cases;}
 fclose(input);fclose(output);output=0;printf("NATIVE_DISPLAY_ENGINE_INIT_PASS cases=%u\n",cases);return 0;
}
