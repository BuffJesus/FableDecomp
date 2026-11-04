#include <stdio.h>
#include "fable_render_state_manager.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned calls,mutation;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,const unsigned* args=0,unsigned count=0)
{
    unsigned header[10]={0};header[0]=kind;
    for(unsigned i=0;i<count;++i)header[i+1]=args[i];
    fwrite(header,4,10,output);fwrite(arena,1,0x6000,output);
}
static long callback(unsigned kind,const unsigned* args,unsigned count)
{
    event(kind,args,count);++calls;
    if(mutation&&kind!=4)
    {
        unsigned index=args[kind==1?1:2]-100;
        if(index>=8){event(99);return -1;}
        word(0x5000+index*0x14+4)=0x76540000+calls;
        word(4)=word(0x3cf0)=(unsigned)(arena+(calls&1?0x5840:0x5800));
    }
    return mutation?0x80004005:0;
}
static long __stdcall state(void* receiver,unsigned state,unsigned value)
{unsigned args[]={(unsigned)receiver,state,value};return callback(1,args,3);}
static long __stdcall texture(void* receiver,unsigned stage,unsigned state,unsigned value)
{unsigned args[]={(unsigned)receiver,stage,state,value};return callback(2,args,4);}
static long __stdcall sampler(void* receiver,unsigned stage,unsigned state,unsigned value)
{unsigned args[]={(unsigned)receiver,stage,state,value};return callback(3,args,4);}
static long __stdcall draw(void* receiver,unsigned type,unsigned min,unsigned vertices,unsigned primitives,void* indices,unsigned format,void* data,unsigned stride)
{unsigned args[]={(unsigned)receiver,type,min,vertices,primitives,(unsigned)indices,format,(unsigned)data,stride};return callback(4,args,9);}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    word(0x8000+57*4)=(unsigned)&state;word(0x8000+67*4)=(unsigned)&texture;
    word(0x8000+69*4)=(unsigned)&sampler;word(0x8000+84*4)=(unsigned)&draw;
    unsigned cases=0;
    while(fread(&mutation,4,1,input)==1)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;calls=0;
        unsigned count=(word(0x2808)/2)-1;
        for(unsigned repeat=0;repeat<2;++repeat)
        {
            ((CRenderManagerCore*)arena)->RestoreCaptureBlock();event(5);
            ((CRenderManagerCore*)arena)->DrawAsIndexedTris(arena+0x5600,12+repeat*4,count==0?0:count+2,arena+0x5700,count);event(6);
        }
        ++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);printf("NATIVE_RENDER_CAPTURE_DRAW_PASS cases=%u\n",cases);return 0;
}
