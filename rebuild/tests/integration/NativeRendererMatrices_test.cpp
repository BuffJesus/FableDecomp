#include <stdio.h>
#include <string.h>
#include <float.h>
#include "renderer_matrices_aliases.h"
#include "renderer_matrices_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SetView(void*,void*,const void*);
extern "C" void __fastcall SetWorld(void*,void*,const void*);
extern "C" void __fastcall SetPerspective(void*,void*,float,float,float,float);
static unsigned char* arena;
static FILE* output;
static unsigned mode,sourceOffset,resultCode;
static void event(unsigned kind,unsigned self=0,unsigned state=0,const void* matrix=0){
    unsigned h[]={kind,self,state};fwrite(h,4,3,output);fwrite(arena,1,0x5000,output);if(matrix)fwrite(matrix,1,64,output);
}
static long __stdcall TransformBoundary(void* self,unsigned state,const void* matrix){
    event(1,(unsigned)self,state,matrix);
    if(mode&1){*(unsigned*)(arena+0x3cf0)=(unsigned)(arena+0x4300);*(unsigned*)(arena+sourceOffset)=0xabcdef01;}
    if(mode&2){((unsigned*)matrix)[0]=0x76543210;*(unsigned*)(arena+0x4000)=0x12345678;}
    event(2,(unsigned)self,state,matrix);return resultCode;
}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;
    memcpy(code,retailView,sizeof(retailView));memcpy(code+256,retailWorld,sizeof(retailWorld));memcpy(code+512,retailPerspective,sizeof(retailPerspective));
    typedef void (__fastcall *MatrixFunction)(void*,void*,const void*);
    typedef void (__fastcall *PerspectiveFunction)(void*,void*,float,float,float,float);
    MatrixFunction view=argv[3][0]=='r'?(MatrixFunction)code:SetView,world=argv[3][0]=='r'?(MatrixFunction)(code+256):SetWorld;
    PerspectiveFunction perspective=argv[3][0]=='r'?(PerspectiveFunction)(code+512):SetPerspective;
    unsigned p[5],cases=0;
    while(fread(p,4,5,in)==5){
        if(fread(arena,1,0x5000,in)!=0x5000)return 5;
        sourceOffset=p[1];mode=p[3];resultCode=p[2];_clearfp();_control87(p[4]?_PC_64:_PC_53,_MCW_PC);
        *(unsigned*)(arena+0x3cf0)=(unsigned)(arena+0x4200);*(unsigned*)(arena+0x4200)=(unsigned)(arena+0x4400);*(unsigned*)(arena+0x4300)=(unsigned)(arena+0x4400);*(unsigned*)(arena+0x44b0)=(unsigned)TransformBoundary;
        if(p[0]==0)view(arena,0,arena+sourceOffset);
        else if(p[0]==1)world(arena,0,arena+sourceOffset);
        else {const float* v=(const float*)(arena+0x4800);perspective(arena,0,v[0],v[1],v[2],v[3]);}
        event(3);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_RENDERER_MATRICES_PASS cases=%u\n",cases);return 0;
}
