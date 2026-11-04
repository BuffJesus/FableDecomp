#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_shader_constant_layout.h"
#include "fable_string.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static FILE* output;static unsigned char* arena;static unsigned emptyVtable,rangeVtable;
static unsigned offsets[]={4,16,28,40,52,64,80,92,104,116,128,140,152,164,176,188,200,212,224,236,248,260,272,284,296,308};
static void event(unsigned kind,unsigned argument=0,const char* text="")
{
 unsigned header[]={kind,argument};char label[16]={0};unsigned char copy[332];
 strncpy(label,text,15);memcpy(copy,&Fable2DShaderConstantLayout_013ca8a8,332);
 if(*(unsigned*)copy==emptyVtable)*(unsigned*)copy=0x129b9b0;
 for(unsigned i=0;i<26;++i)if(*(unsigned*)(copy+offsets[i])==rangeVtable)*(unsigned*)(copy+offsets[i])=0x129a04c;
 fwrite(header,4,2,output);fwrite(label,1,16,output);fwrite(copy,1,332,output);
}
extern "C" void __cdecl FableLayoutGlobalFree(void* pointer){event(4,(unsigned)pointer);}
void __cdecl operator delete(void* pointer){if(output)event(5,(unsigned)pointer);}
CCharString::CCharString(const char* text,long length)
{event(2,(unsigned)length,text);storage_=(CCharStringData*)(arena+(*text?0x2010:0x2000));}
CCharString::CCharString(const CCharString& other):storage_(other.storage_){}
CCharString::~CCharString(){}
typedef void* (__fastcall* DeletingDestructor)(void*,void*,unsigned);
int main(int argc,char** argv)
{
 if(argc!=2)return 2;output=fopen(argv[1],"wb");if(!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 emptyVtable=*(unsigned*)&Fable2DShaderConstantLayout_013ca8a8;rangeVtable=*(unsigned*)((char*)&Fable2DShaderConstantLayout_013ca8a8+4);event(1);
 CVertexShaderConstantLayout* base=new(arena+0x1000) CVertexShaderConstantLayout;
 for(unsigned i=0;i<2;++i)
 {
  CVertexShaderConstantLayout* layout=i?&Fable2DShaderConstantLayout_013ca8a8:base;
  CCharString name=layout->PeekName();event(3,*(unsigned*)&name);
 }
 for(unsigned flag=0;flag<2;++flag)
 {
  CVertexShaderConstantLayoutEmpty* layout=new(arena+0x1800+flag*0x400) CVertexShaderConstantLayoutEmpty;
  DeletingDestructor destroy=(DeletingDestructor)(*(void***)layout)[0];event(6,(unsigned)destroy(layout,0,flag));
 }
 unsigned* presets=(unsigned*)((char*)&Fable2DShaderConstantLayout_013ca8a8+0x140);
 presets[0]=(unsigned)(arena+0x400);presets[1]=(unsigned)(arena+0x420);presets[2]=(unsigned)(arena+0x440);
 // Keep the stream open for the real CRT-registered global destructor.
 // CRT stdio cleanup flushes it after the atexit callbacks have run.
 if(fflush(output))return 5;printf("NATIVE_SHADER_LAYOUT_GLOBAL_PASS\n");return 0;
}
