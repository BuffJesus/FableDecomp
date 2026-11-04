#include <stdio.h>
#include "fable_shader_render_manager.h"
#include "fable_display_viewport.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);

static unsigned char* arena;
static FILE* output;
static unsigned mutation;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned value=0)
{
    unsigned header[]={kind,value};fwrite(header,4,2,output);fwrite(arena,1,0x6000,output);
}
static long __stdcall clearPixelShader(void* receiver,void* shader)
{
    if(receiver!=arena+0x900||shader)event(99);
    event(10);
    if(mutation==1){word(0x1e4)=(unsigned)(arena+0x840);word(0x3d8)=0x1234567f;}
    else if(mutation==2){word(0x1e4)=0;arena[0x391]=255;}
    else if(mutation==3){word(0x808)=1;word(0x3d8)=0xfedcba9b;}
    return mutation&1?0x80004005:0;
}
class ReleaseBoundary
{
public:
    virtual void Destroy(unsigned flags)
    {
        if(((void*)this!=arena+0x800&&(void*)this!=arena+0x840)||flags!=1)event(99);
        event(11,(unsigned)this);
        if(mutation){word(0x1e4)=(unsigned)(arena+0x880);word(0x3d8)=0x10203040;}
    }
};

int main(int argc,char** argv)
{
    if(argc!=3)return 2;
    FILE* input=fopen(argv[1],"rb");
    output=fopen(argv[2],"wb");
    if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);
    if(arena!=(void*)0x24000000)return 4;
    unsigned cases=0;
    ReleaseBoundary release;
    word(0x8000+107*4)=(unsigned)&clearPixelShader;
    word(0x9000)=(*(unsigned**)&release)[0];
    unsigned params[2];
    const unsigned sequence[]={0,1,2,0,3,4,1,2};
    while(fread(params,4,2,input)==2)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 8;
        mutation=params[1];
        CShaderRenderManager* shader=(CShaderRenderManager*)arena;
        if(params[0]){shader->DisablePixelShaders();event(12);++cases;continue;}
        CRenderManagerCore* core=(CRenderManagerCore*)(arena+0x1000);
        CDisplayManager* display=(CDisplayManager*)(arena+0x5000);
        bool initial=arena[0x390]!=0;
        for(unsigned i=0;i<sizeof(sequence)/sizeof(sequence[0]);++i)
        {
            unsigned result=0,op=sequence[i];
            if(op==0)shader->EnablePixelShaders();
            else if(op==1)shader->EnableVertexShaders();
            else if(op==2)shader->DisableVertexShaders();
            else if(op==3)result=display->GetPixelShaderVersion();
            else core->EnableXBoxShaderConstants(initial);
            unsigned header[]={op,result};
            if(fwrite(header,4,2,output)!=2||fwrite(arena,1,0x6000,output)!=0x6000)return 5;
        }
        ++cases;
    }
    if(ferror(input))return 6;
    fclose(input);
    if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);
    printf("NATIVE_SHADER_ENABLE_PASS cases=%u\n",cases);
    return 0;
}
