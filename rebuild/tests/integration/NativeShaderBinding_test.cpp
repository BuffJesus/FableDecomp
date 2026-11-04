#include <stdio.h>
#include "fable_shader_render_manager.h"
#include "fable_vertex_shader.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned calls,mutation;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned receiver=0,unsigned value=0)
{
    unsigned header[]={kind,receiver,value};fwrite(header,4,3,output);fwrite(arena,1,0x6000,output);
}
static long callback(unsigned kind,void* receiver,void* value)
{
    if(receiver!=arena+0x900&&receiver!=arena+0x940)event(99);
    event(kind,(unsigned)receiver,(unsigned)value);++calls;
    if(mutation)
    {
        word(0x1d4)=(unsigned)(arena+(calls&1?0x940:0x900));
        word(0x1dc)=0xabcdef00+calls;word(0x1d8)=0x12340000+calls;
        word(0x704)=(unsigned)(arena+0x780);word(0x3908)=calls&1;arena[0x4cfd]=(unsigned char)(calls&1);
    }
    return mutation?0x80004005:0;
}
static long __stdcall setShader(void* receiver,void* shader){return callback(1,receiver,shader);}
static long __stdcall setDeclaration(void* receiver,void* declaration){return callback(2,receiver,declaration);}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    word(0x8000+92*4)=(unsigned)&setShader;word(0x8000+87*4)=(unsigned)&setDeclaration;
    unsigned params[2],cases=0;
    while(fread(params,4,2,input)==2)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;mutation=params[0];calls=0;
        for(unsigned repeat=0;repeat<2;++repeat)
        {
            ((CShaderRenderManager*)arena)->ApplyVertexShader(*(CVertexShader*)(arena+0x700),params[1]!=0);
            event(3);
        }
        ++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);printf("NATIVE_SHADER_BINDING_PASS cases=%u\n",cases);return 0;
}
