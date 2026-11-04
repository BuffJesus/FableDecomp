#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "renderer_dtor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall DestroyRenderer(void*);
extern "C" void* __fastcall DeleteRenderer(void*,void*,unsigned);
extern "C" void __fastcall DestroyRecords(void*);
extern "C" unsigned char VertexTable[],ShaderTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0){
 unsigned h[]={kind,a,b};fwrite(h,4,3,output);unsigned save=word(0x3e50),c=word(0x5104),d=word(0x5144);
 if(save==(unsigned)VertexTable)word(0x3e50)=0x129b3d0;else if(save==(unsigned)ShaderTable)word(0x3e50)=0x129a030;
 word(0x5104)=word(0x5144)=0;fwrite(arena,1,0x6000,output);word(0x3e50)=save;word(0x5104)=c;word(0x5144)=d;
}
extern "C" void __fastcall TextureBoundary(void* self){event(1,(unsigned)self);*(unsigned*)self=0;if(mode==1){unsigned v[]={0x3e64,0x3e7c,0x3e94};for(int i=0;i<3;++i)word(v[i])=word(v[i]+4)=(unsigned)(arena+0x4800);}}
extern "C" void __fastcall WideBoundary(void* self){event(2,(unsigned)self);*(unsigned*)self=0;}
extern "C" void __cdecl FableRendererFree(void* pointer){event(3,(unsigned)pointer);if(mode==2){word(0x3e58)=word(0x3e70)=word(0x3e88)=(unsigned)(arena+0x4a00);}}
void operator delete(void* pointer){event(6,(unsigned)pointer);}
extern "C" void __fastcall BankBoundary(void* self){event(7,(unsigned)self);*(unsigned*)self=0x12344321;}
extern "C" void __fastcall CoreBoundary(void* self){event(8,(unsigned)self);*(unsigned*)self=0x87654321;}
extern "C" void __fastcall ShaderDelete(void* self,void*,unsigned flags){event(4,(unsigned)self,flags);if(mode==3)word(0x3e4c)=(unsigned)(arena+0x5140);}
extern "C" void __fastcall ControlRelease(void* self){event(5,(unsigned)self);if(mode)word(0x3e4c)=(unsigned)(arena+0x5140);}
extern "C" void UnusedRendererDtorBoundary(){abort();}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8000)=(unsigned)&ShaderDelete;
 unsigned p[8],cases=0;while(fread(p,4,8,input)==8){if(fread(arena,1,0x6000,input)!=0x6000)return 5;mode=p[7];word(0x5104)=word(0x5144)=(unsigned)&ControlRelease;unsigned result=0;
 if(p[0]==0)DestroyRenderer(arena);else if(p[0]==2)DestroyRecords(arena+0x3e94);else result=(unsigned)DeleteRenderer(arena,0,p[0]==1?1:p[0]==3?0:p[0]==4?2:0xffffffff);
 event(9,p[0],result);++cases;}
 fclose(input);fclose(output);printf("NATIVE_RENDERER_DTOR_PASS cases=%u\n",cases);return 0;
}
