#include <stdio.h>
#include "fable_render2d_vertex.h"
#include "fable_stlport_vector.h"
namespace _STL { template<> CTVertexRHWColSpecTex1* vector<CTVertexRHWColSpecTex1>::erase(CTVertexRHWColSpecTex1*,CTVertexRHWColSpecTex1*); }
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
 unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned first,last,cases=0;
 while(fread(&first,4,1,input)==1)
 {
  if(fread(&last,4,1,input)!=1||fread(arena,1,0x1000,input)!=0x1000)return 5;
  _STL::vector<CTVertexRHWColSpecTex1>* v=(_STL::vector<CTVertexRHWColSpecTex1>*)arena;
  CTVertexRHWColSpecTex1* records=(CTVertexRHWColSpecTex1*)(arena+0x100);
  unsigned result=(unsigned)v->erase(records+first,records+last);
  fwrite(&result,4,1,output);fwrite(arena,1,0x1000,output);++cases;
 }
 if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
 printf("NATIVE_RENDER2D_VERTEX_ERASE_PASS cases=%u\n",cases);return 0;
}
