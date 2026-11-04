#include <stdio.h>
#include "fable_shader_render_manager.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned allocations;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned value=0)
{
    unsigned header[]={kind,value};fwrite(header,4,2,output);fwrite(arena,1,0x6000,output);
}
extern "C" void* __cdecl FableShaderLayoutTestMalloc(size_t size)
{
    event(1,(unsigned)size);if(size>0x1000||allocations>=4)return 0;
    return arena+0x2000+allocations++*0x1000;
}
extern "C" void __cdecl FableShaderLayoutTestFree(void* p){event(2,(unsigned)p);}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[5],cases=0;
    while(fread(params,4,5,input)==5)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;allocations=0;
        CShaderRenderManager* shader=(CShaderRenderManager*)arena;
        _STL::vector<const CVertexShaderConstantLayout*>& v=shader->ConstantLayouts();
        const CVertexShaderConstantLayout* const& value=*(const CVertexShaderConstantLayout**)params[3];
        if(params[0]==0)
        {
            for(unsigned n=0;n<2;++n)
            {
                shader->RegisterVertexShaderConstantLayout(params[1]+n*2,*(const CVertexShaderConstantLayout**)(arena+0x800));event(10);
                shader->UnregisterVertexShaderConstantLayouts();event(11);
            }
        }
        else if(params[0]==1){v.resize(params[1],value);event(12);}
        else if(params[0]==2){v._M_fill_insert(v.begin()+params[2],params[1],value);event(13);}
        else{v._M_insert_overflow(v.begin()+params[2],value,_STL::__true_type(),params[1],params[4]!=0);event(14);}
        ++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);printf("NATIVE_SHADER_LAYOUT_VECTOR_PASS cases=%u\n",cases);return 0;
}
