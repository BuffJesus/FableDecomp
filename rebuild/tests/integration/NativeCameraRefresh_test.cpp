#include <stdio.h>
#include <string.h>
#include <float.h>
#include "camera_refresh_aliases.h"
#include "camera_refresh_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RefreshCamera(void*);
#ifdef COMPOSED_SHADER
extern "C" void __fastcall ShaderView(void*,void*,const void*);
extern "C" void __fastcall ShaderProjection(void*,void*,const void*);
extern "C" void __fastcall ShaderPosition(void*,void*,const void*);
extern "C" void __fastcall ShaderWorld(void*,void*,const void*);
#endif
extern "C" void* RenderGlobal;
extern "C" void* ShaderGlobal;
#ifdef COMPOSED_RENDERER
extern "C" void __fastcall RendererView(void*,void*,const void*);
extern "C" void __fastcall RendererWorld(void*,void*,const void*);
extern "C" void __fastcall RendererPerspective(void*,void*,float,float,float,float);
static const unsigned arenaSize=49152,renderA=0x1000,renderB=0x5000,shaderA=0x9000,shaderB=0x9800;
#else
static const unsigned arenaSize=4096,renderA=0x400,renderB=0x500,shaderA=0x600,shaderB=0x700;
#endif
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static void put(unsigned offset,unsigned value){*(unsigned*)(arena+offset)=value;}
static void record(unsigned kind,void* self,const void* data=0,unsigned size=0){
    unsigned h[]={kind,(unsigned)self,(unsigned)RenderGlobal,(unsigned)ShaderGlobal,size};
    fwrite(h,4,5,output);fwrite(arena,1,arenaSize,output);if(size)fwrite(data,1,size,output);
    if(mode&1){RenderGlobal=arena+((kind&1)?renderB:renderA);ShaderGlobal=arena+((kind&1)?shaderB:shaderA);}
    if(mode&2){put(0x6c,0x3f000000+kind);put(0x54,0x40000000+kind);put(0x80,0x40400000+kind);put(0x174,0x40800000+kind);}
    if((mode&4)&&kind==8){unsigned* matrix=(unsigned*)data;matrix[0]=0x41200000;matrix[11]=0x80000000;}
}
extern "C" void __fastcall BoxBoundary(const void* input,void* result){record(1,0,input,16);memcpy(result,input,16);}
extern "C" void __fastcall WindowBoundary(void* self,void*,const void* p){record(2,self,p,16);}
extern "C" void __fastcall ViewBoundary(void* self,void*,const void* p){record(3,self,p,48);}
extern "C" void __fastcall PerspectiveBoundary(void* self,void*,float a,float b,float c,float d){float values[]={a,b,c,d};record(4,self,values,16);}
extern "C" void __fastcall ShaderViewBoundary(void* self,void*,const void* p){record(5,self,p,48);}
extern "C" void __fastcall ProjectionBoundary(void* self,void*,const void* p){record(6,self,p,64);}
extern "C" void __fastcall PositionBoundary(void* self,void*,const void* p){record(7,self,p,12);}
extern "C" void __fastcall WorldBoundary(void* self,void*,const void* p){record(8,self,p,48);}
extern "C" void __fastcall ShaderWorldBoundary(void* self,void*,const void* p){record(9,self,p,48);}
#ifdef COMPOSED_RENDERER
static long __stdcall SetTransform(void* self,unsigned state,const void* matrix){
    if(state!=2&&state!=3&&state!=256)throw 2;
    record(state==2?3:state==3?4:8,self,matrix,64);
    return (mode&4)?(long)0x8876086c:0;
}
static void* deviceTable[45];
#endif
static unsigned resolve(unsigned address){
    switch(address){
    case 0x9e2020:return (unsigned)BoxBoundary;
    case 0x1436e18:return (unsigned)&RenderGlobal;
    case 0x1436e14:return (unsigned)&ShaderGlobal;
    case 0xa0b560:return (unsigned)WindowBoundary;
#ifdef COMPOSED_RENDERER
    case 0xa0b0a0:return (unsigned)RendererView;
    case 0xa0b320:return (unsigned)RendererPerspective;
    case 0xa0b140:return (unsigned)RendererWorld;
#else
    case 0xa0b0a0:return (unsigned)ViewBoundary;
    case 0xa0b320:return (unsigned)PerspectiveBoundary;
    case 0xa0b140:return (unsigned)WorldBoundary;
#endif
#ifdef COMPOSED_SHADER
    case 0x988350:return (unsigned)ShaderView;
    case 0x988540:return (unsigned)ShaderProjection;
    case 0x988320:return (unsigned)ShaderPosition;
    case 0x9881f0:return (unsigned)ShaderWorld;
#else
    case 0x988350:return (unsigned)ShaderViewBoundary;
    case 0x988540:return (unsigned)ProjectionBoundary;
    case 0x988320:return (unsigned)PositionBoundary;
    case 0x9881f0:return (unsigned)ShaderWorldBoundary;
#endif
    }
    throw 1;
}
int main(int argc,char** argv){
    if(argc!=4||RenderGlobal||ShaderGlobal)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned offset=retailFixups[i][0],value=resolve(retailFixups[i][1]);if(retailFixups[i][2])value-=(unsigned)(code+offset+4);*(unsigned*)(code+offset)=value;
    }
    typedef void (__fastcall *RefreshFunction)(void*);
    RefreshFunction refresh=argv[3][0]=='r'?(RefreshFunction)code:RefreshCamera;unsigned cases=0;
    while(fread(&mode,4,1,in)==1){
        if(fread(arena,1,arenaSize,in)!=arenaSize)return 5;
#ifdef COMPOSED_RENDERER
        deviceTable[44]=(void*)SetTransform;
        put(0xa000,(unsigned)deviceTable);put(0xa100,(unsigned)deviceTable);
        put(renderA+0x3cf0,(unsigned)(arena+0xa000));put(renderB+0x3cf0,(unsigned)(arena+0xa100));
#endif
        _clearfp();_control87((mode&8)?_PC_64:_PC_53,_MCW_PC);
        RenderGlobal=arena+renderA;ShaderGlobal=arena+shaderA;refresh(arena);record(10,arena);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_CAMERA_REFRESH_PASS cases=%u\n",cases);return 0;
}
