#include <stdio.h>
#include "fable_render_state_manager.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation,phase,calls;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned receiver=0,unsigned stage=0,unsigned texture=0)
{unsigned args[]={kind,phase,receiver,stage,texture};fwrite(args,4,5,output);fwrite(arena,1,0x5000,output);}
static void mutate(unsigned kind,unsigned stage)
{
 ++calls;if(!mutation)return;
 word(0x4004)^=0x76543210;word(0x4014)+=0x01000011;
 if(phase==0)word(0x4010)=(unsigned)(arena+(calls&1?0x4100:0x4120));
 if(kind==3){word(0x3d00+stage*4)=(unsigned)(arena+0x4120);word(0x3cb8)=calls&1?0xffffffff:8;}
}
static unsigned long __stdcall release(void* receiver){event(1,(unsigned)receiver);mutate(1,0);return 7;}
static unsigned long __stdcall addref(void* receiver){event(2,(unsigned)receiver);mutate(2,0);return 9;}
static long __stdcall setTexture(void* receiver,unsigned stage,void* texture)
{event(3,(unsigned)receiver,stage,(unsigned)texture);mutate(3,stage);return mutation?0x80004005:0;}
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 word(0x8004)=(unsigned)&addref;word(0x8008)=(unsigned)&release;word(0x8400+65*4)=(unsigned)&setTexture;
 unsigned self,stage,cases=0;
 while(fread(&mutation,4,1,input)==1)
 {
  if(fread(&self,4,1,input)!=1 || fread(&stage,4,1,input)!=1 || fread(arena,1,0x5000,input)!=0x5000)return 5;
  calls=0;CTexture* destination=(CTexture*)(arena+0x4000);CTexture* source=(CTexture*)(arena+(self?0x4000:0x4010));CRenderManagerCore* core=(CRenderManagerCore*)arena;
  phase=0;const CTexture& result=(*destination=*source);event(4,(unsigned)&result);
  phase=1;core->AttachTextureToStage(stage,destination);event(4);
  phase=2;core->AttachTextureToStage(stage,destination);event(4);
  phase=3;destination->Uninitialise();event(4);
  phase=4;core->AttachTextureToStage(stage,destination);event(4);
  phase=5;source->Uninitialise();event(4);++cases;
 }
 if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
 printf("NATIVE_TEXTURE_BINDINGS_PASS cases=%u\n",cases);return 0;
}
