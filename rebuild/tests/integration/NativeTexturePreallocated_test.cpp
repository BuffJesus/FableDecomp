#include <stdio.h>
#include <string.h>
#include "fable_surface_types.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation,dimensions,calls;static long levels;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned receiver=0,unsigned value=0)
{unsigned args[]={kind,receiver,value};fwrite(args,4,3,output);fwrite(arena,1,0x1000,output);}
static void mutate(unsigned kind)
{++calls;if(mutation){word(4)^=0xa7654321;if(kind==2||kind==3)word(0)=(unsigned)arena+(calls&1?0x200:0x100);}}
static unsigned long __stdcall release(void* receiver){event(1,(unsigned)receiver);mutate(1);return 7;}
static unsigned long __stdcall getCount(void* receiver){event(2,(unsigned)receiver);mutate(2);return levels;}
static long __stdcall getDesc(void* receiver,unsigned level,D3DSURFACE_DESC* desc)
{
 event(3,(unsigned)receiver,level);mutate(3);unsigned width,height;
 if(dimensions==0){width=0;height=17;}else if(dimensions==1){width=257>>level;height=129>>level;}
 else {width=0xfffffff1-level;height=0x81234567+level;}
 unsigned values[]={0x15+calls%3,1,2,3,4,5,width,height};memcpy(desc,values,32);return mutation?0x80004005:0;
}
void CPixelFormat::Initialise(D3DFORMAT format){event(4,0,(unsigned)format);mutate(4);FormatIndex=(unsigned)format^0x13579;}
unsigned long CPixelFormat::GetColourDepth()const
{event(5,0,(unsigned)FormatIndex);mutate(5);unsigned values[]={0,1,8,24,32,0x10000001};return values[(unsigned)FormatIndex%6];}
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 word(0x8008)=(unsigned)&release;word(0x8000+13*4)=(unsigned)&getCount;word(0x8000+17*4)=(unsigned)&getDesc;
 unsigned next,reserved,cases=0;
 while(fread(&next,4,1,input)==1)
 {
  if(fread(&levels,4,1,input)!=1||fread(&dimensions,4,1,input)!=1||fread(&mutation,4,1,input)!=1||fread(&reserved,4,1,input)!=1||fread(arena,1,0x1000,input)!=0x1000)return 5;
  calls=0;CTexture* texture=(CTexture*)arena;
  event(6,0,texture->InitialiseFromPreallocatedTexture(next?(IDirect3DTexture9*)(arena+0x200):0));
  event(6,0,texture->InitialiseFromPreallocatedTexture(0));++cases;
 }
 if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
 printf("NATIVE_TEXTURE_PREALLOCATED_PASS cases=%u\n",cases);return 0;
}
