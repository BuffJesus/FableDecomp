#include "fable_shader_render_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
#pragma comment(linker,"/alternatename:??0CBaseClassNonCopyable@@QAE@XZ=_UnusedShaderShutdownBoundary")
#pragma comment(linker,"/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=_UnusedShaderShutdownBoundary")
#pragma comment(linker,"/alternatename:??1CBaseClass@@UAE@XZ=_UnusedShaderShutdownBoundary")
#pragma comment(linker,"/alternatename:??0CVertexShaderInput@@QAE@XZ=_UnusedShaderShutdownBoundary")
#pragma comment(linker,"/alternatename:??1CVertexShaderInput@@QAE@XZ=_UnusedShaderShutdownBoundary")
extern "C" void UnusedShaderShutdownBoundary(){abort();}
static unsigned char* arena;static FILE* output;static unsigned mode,count,deletes;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0){unsigned h[]={kind,a,b};fwrite(h,4,3,output);fwrite(arena,1,0x4000,output);}
extern "C" void* __fastcall DeleteShader(void* self,void*,unsigned flags){event(1,(unsigned)self,flags);++deletes;
 if(mode==1){word(0x3a8)=(unsigned)(arena+0x600);word(0x3ac)=(unsigned)(arena+0x600)+count*8;}
 else if(mode==2)word(0x3ac)=word(0x3a8)+8;
 else if(mode==3&&deletes==1)word(0x3ac)=word(0x3a8)+(count+1)*8;
 else if(mode==4){unsigned* resource=*(unsigned**)((char*)self+4);resource[2]=(unsigned)(arena+0x2200);resource[3]=(unsigned)(arena+0x2204);}
 word(0x1d4)=0xabcdef12;*(unsigned*)((char*)self+8)=0x98765432;return self;
}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8000)=(unsigned)&DeleteShader;
 unsigned p[4],cases=0;while(fread(p,4,4,input)==4){if(fread(arena,1,0x4000,input)!=0x4000)return 5;count=p[0];mode=p[3];deletes=0;((CShaderRenderManager*)arena)->Uninitialise();event(2);++cases;}
 fclose(input);fclose(output);printf("NATIVE_SHADER_SHUTDOWN_PASS cases=%u\n",cases);return 0;
}
