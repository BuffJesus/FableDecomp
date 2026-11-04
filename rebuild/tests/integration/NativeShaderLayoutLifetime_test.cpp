#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_shader_constant_layout.h"
#include "fable_string.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation;
static unsigned baseVtable,layoutVtable,rangeVtable;
static unsigned offsets[]={0,4,16,28,40,52,64,80,92,104,116,128,140,152,164,176,188,200,212,224,236,248,260,272,284,296,308};
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned pointer=0)
{
 unsigned header[]={kind,pointer};unsigned char copy[4096];memcpy(copy,arena,sizeof(copy));
 for(unsigned i=0;i<sizeof(offsets)/sizeof(offsets[0]);++i)
 {
  unsigned& value=*(unsigned*)(copy+offsets[i]);
  if(value==baseVtable)value=0x1231710;
  else if(value==layoutVtable)value=0x129a06c;
  else if(value==rangeVtable)value=0x129a04c;
 }
 fwrite(header,4,2,output);fwrite(copy,1,sizeof(copy),output);
}
extern "C" void __cdecl FableLayoutFixtureFree(void* pointer)
{
 event(2,(unsigned)pointer);
 if(mutation){word(0x140)=0x55667788;word(8)=0x87654321;}
}
void __cdecl operator delete(void* pointer)
{if(output)event(3,(unsigned)pointer);}
// Required virtual name slot is never invoked by this lifetime fixture.
CCharString CVertexShaderConstantLayout::PeekName(){exit(90);}
typedef void* (__fastcall* DeletingDestructor)(void*,void*,unsigned);
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 CBaseClass base;baseVtable=*(unsigned*)&base;
 unsigned operation,count,cases=0;
 while(fread(&operation,4,1,input)==1)
 {
  if(fread(&count,4,1,input)!=1 || fread(&mutation,4,1,input)!=1 || fread(arena,1,4096,input)!=4096)return 5;
  CVertexShaderConstantLayout* layout=new(arena) CVertexShaderConstantLayout;
  layoutVtable=word(0);rangeVtable=word(4);event(1);
  word(0x140)=count?(unsigned)(arena+0x400):0;word(0x144)=count?(unsigned)(arena+0x400+count*16):0;word(0x148)=word(0x144);
  unsigned offset=operation<2?0:offsets[1+(cases%26)];
  void* target=arena+offset;DeletingDestructor destroy=(DeletingDestructor)(*(void***)target)[0];
  void* result=destroy(target,0,operation&1);event(4,(unsigned)result);++cases;
 }
 if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;output=0;
 printf("NATIVE_SHADER_LAYOUT_LIFETIME_PASS cases=%u\n",cases);return 0;
}
