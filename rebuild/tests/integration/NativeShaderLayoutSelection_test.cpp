#include <stdio.h>
#include <string.h>
#include "fable_shader_render_manager.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0,const void* value=0)
{
    unsigned header[]={kind,a,b,c};unsigned zero[4]={0,0,0,0};
    fwrite(header,4,4,output);fwrite(value?value:zero,1,16,output);fwrite(arena,1,0x6000,output);
}
void CShaderRenderManager::AllocateLights(unsigned long count,unsigned long size)
{
    if(this!=(void*)arena||count>8||size>2)event(99);
    event(1,count,size);Lights=(CLightInfo*)(arena+0x3000);LightBuffer=(C4DVector*)(arena+0x4000);
    LightCount=count+(mutation==1&&count<4?1:0);AllocatedLights=8;AllocatedLightSize=4;
    if(mutation==2){word(4)=(unsigned)(arena+0x1600);word(0x3d8)^=0x40000040;}
}
namespace NVectorMath {
void __cdecl GFClear4DVectors(C4DVector* target,unsigned long count,const C4DVector& value)
{
    if((unsigned char*)target<arena+0x4000||(unsigned char*)target>=arena+0x4800||count>32)event(99);
    event(2,(unsigned)target,count,0,&value);for(unsigned i=0;i<count;++i)memcpy(target+i,&value,16);
    if(mutation==1){word(4)=(unsigned)(arena+0x1600);word(0x3d8)^=0x80;}
    if(mutation==2)word(0x198)=word(0x194);
}
}
#ifndef FABLE_SELECTION_NATIVE_VECTOR
namespace _STL {
template<> void vector<C4DVector>::resize(size_t count,const C4DVector& value)
{
    if(this!=(void*)(arena+0x194)||count>8)event(99);
    event(3,count,0,0,&value);unsigned old=(word(0x198)-word(0x194))/16,copied=old<count?old:count;
    if(copied)memcpy(arena+0x5000,(void*)word(0x194),copied*16);
    for(unsigned i=copied;i<count;++i)memcpy(arena+0x5000+i*16,&value,16);
    word(0x194)=(unsigned)(arena+0x5000);word(0x198)=(unsigned)(arena+0x5000+count*16);word(0x19c)=(unsigned)(arena+0x5080);
    if(mutation){word(4)=(unsigned)(arena+0x1600);word(0x3d8)^=0x40000000;}
}
}
#else
extern "C" void* __cdecl FableVector4TestMalloc(size_t size)
{event(4,(unsigned)size);return size<=0x800?arena+0x5000:0;}
extern "C" void __cdecl FableVector4TestFree(void* p){event(5,(unsigned)p);}
#endif
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[2],cases=0;
    while(fread(params,4,2,input)==2)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;mutation=params[1];
        ((CShaderRenderManager*)arena)->SetVertexShaderConstantLayout((long)params[0]);event(9);++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);printf("NATIVE_SHADER_LAYOUT_SELECTION_PASS cases=%u\n",cases);return 0;
}
