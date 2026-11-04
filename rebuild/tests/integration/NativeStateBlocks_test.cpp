#include <stdio.h>
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation,index;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind)
{unsigned args[]={kind,index};fwrite(args,4,2,output);fwrite(arena,1,0xa000,output);}
CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
 event(1);if(mutation)word(0x8208)=(unsigned)arena+(word(0x8208)==(unsigned)arena?0x4000:0);
 return (CSystemManager*)(arena+0x8000);
}
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x20000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 if(VirtualAlloc((void*)0x1390000,0x10000,0x3000,4)!=(void*)0x1390000)return 5;
 if(fread((void*)0x1390000,1,0x10000,input)!=0x10000)return 6;
 if(CRenderManager2D::LibraryStateBlockFunctions[0] || CRenderManager2D::LibraryStateBlockFunctions[6])return 7;
 unsigned reverse,cases=0;
 while(fread(&mutation,4,1,input)==1)
 {
  if(fread(&reverse,4,1,input)!=1 || fread(arena,1,0xa000,input)!=0xa000)return 8;
  for(unsigned step=0;step<10;++step){index=reverse?4-step%5:step%5;CRenderManager2D::LibraryStateBlockFunctions[index+1]->Apply();event(2);}
  ++cases;
 }
 if(ferror(input))return 9;fclose(input);if(fclose(output))return 10;
 printf("NATIVE_STATE_BLOCKS_PASS cases=%u\n",cases);return 0;
}
