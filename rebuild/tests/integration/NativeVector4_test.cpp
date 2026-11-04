#include <stdio.h>
#include "fable_4d_vector.h"
#include "fable_stlport_vector.h"
namespace _STL {template<> C4DVector* vector<C4DVector>::erase(C4DVector*,C4DVector*);}
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned allocations;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned value=0)
{
    unsigned header[]={kind,value};fwrite(header,4,2,output);fwrite(arena,1,0x6000,output);
}
extern "C" void* __cdecl FableVector4TestMalloc(size_t size)
{
    event(1,(unsigned)size);if(size>0x1000||allocations>=4)return 0;
    return arena+0x2000+allocations++*0x1000;
}
extern "C" void __cdecl FableVector4TestFree(void* p){event(2,(unsigned)p);}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[5],cases=0;
    while(fread(params,4,5,input)==5)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;allocations=0;
        _STL::vector<C4DVector>& v=*(_STL::vector<C4DVector>*)(arena+0xc);
        const C4DVector& value=*(const C4DVector*)params[3];
        if(params[0]==0){C4DVector* first=v.begin()+params[2];event(10,(unsigned)v.erase(first,first+params[1]));}
        else if(params[0]==1){v.resize(params[1],value);event(12);}
        else if(params[0]==2){v._M_fill_insert(v.begin()+params[2],params[1],value);event(13);}
        else{v._M_insert_overflow(v.begin()+params[2],value,_STL::__false_type(),params[1],params[4]!=0);event(14);}
        ++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);printf("NATIVE_VECTOR4_PASS cases=%u\n",cases);return 0;
}
