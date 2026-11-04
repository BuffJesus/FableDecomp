#include <stdio.h>
#include <string.h>
#include <float.h>
#include "fable_shader_render_manager.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;
static FILE* output;
static unsigned calls,mutation;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned receiver=0,unsigned start=0,unsigned count=0,const void* data=0)
{
    unsigned header[]={kind,receiver,start,count};unsigned char payload[128];memset(payload,0,sizeof(payload));
    if(count>8){header[0]=99;count=0;}if(count)memcpy(payload,data,count*16);
    fwrite(header,4,4,output);fwrite(payload,1,128,output);fwrite(arena,1,0x3000,output);
}
static void mutate()
{
    ++calls;if(!mutation)return;
    word(0x3d8)=0xa5a50000|calls*0x407;
    word(4)=(unsigned)(arena+(calls&1?0x1200:0x1000));
    word(0x1d4)=(unsigned)(arena+(calls&1?0x840:0x800));
    word(0x1e4)=(unsigned)(arena+0x2040);word(0x194)=(unsigned)(arena+0x2500);
    word(0x3cc)=(unsigned)(arena+0x2700);
    word(0x1ac)=0x3f000001+calls;word(0x1bc)=0xbf000001+calls;
    arena[0x1e8]=0;word(0x1f0)=0xffc12345;
}
static long __stdcall setConstants(void* receiver,long start,const void* data,unsigned count)
{
    if((receiver!=arena+0x800&&receiver!=arena+0x840)||!count)event(99);
    event(1,(unsigned)receiver,start,count,data);mutate();return mutation?0x80004005:0;
}
static long __stdcall setPixel(void* receiver,void* shader)
{
    if(receiver!=arena+0x800&&receiver!=arena+0x840)event(99);
    event(2,(unsigned)receiver,(unsigned)shader);mutate();return mutation?0x80004005:0;
}
static long __stdcall setPixelConstants(void* receiver,long start,const void* data,unsigned count)
{
    if((receiver!=arena+0x800&&receiver!=arena+0x840)||!count||start!=0)event(99);
    event(3,(unsigned)receiver,start,count,data);mutate();return mutation?0x80004005:0;
}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    word(0x8000+94*4)=(unsigned)&setConstants;word(0x8000+107*4)=(unsigned)&setPixel;
    word(0x8000+109*4)=(unsigned)&setPixelConstants;
    const unsigned sequence[]={6,5,0,1,2,3,4,5,6,1,2,3,4,5,6};
    const unsigned modes[]={_RC_NEAR,_RC_DOWN,_RC_UP,_RC_CHOP};const long starts[]={-1,0,95,255};
    unsigned params[4],cases=0;
    while(fread(params,4,4,input)==4)
    {
        if(fread(arena,1,0x3000,input)!=0x3000)return 5;
        mutation=params[0];calls=0;_controlfp(modes[params[1]],_MCW_RC);
        CShaderRenderManager* shader=(CShaderRenderManager*)arena;
        for(unsigned i=0;i<sizeof(sequence)/sizeof(sequence[0]);++i)
        {
            unsigned op=sequence[i];
            if(op==0)shader->SetVSConstants(starts[params[3]],arena+0x2800,params[2]);
            else if(op==1)shader->UpdatePixelShader();
            else if(op==2)shader->UpdateAmbient();
            else if(op==3)shader->UpdateFogColour();
            else if(op==4)shader->UpdateLightGlobals();
            else if(op==5)shader->UpdatePixelShaderConstants();
            else shader->ResetWorldTransform();
            event(10+op);
        }
        ++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);printf("NATIVE_SHADER_UPDATES_PASS cases=%u\n",cases);return 0;
}
