#include <stdio.h>
#include <string.h>
#include <float.h>
#include "engine_camera_process_aliases.h"
#include "engine_camera_process_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ProcessCamera(void*,void*,bool);
extern "C" unsigned __fastcall Flags(void*);
extern "C" float __fastcall Horizontal(void*);
extern "C" float __fastcall Vertical(void*);
extern "C" void* __fastcall Normalize(void*);
extern "C" const unsigned ZStart;
static unsigned char* arena;
static FILE* output;
static unsigned mode;
static void put(unsigned offset,unsigned value){*(unsigned*)(arena+offset)=value;}
static void event(unsigned kind,void* self,unsigned first=0,unsigned second=0){
    unsigned h[]={kind,(unsigned)self,first,second};fwrite(h,4,4,output);fwrite(arena,1,4096,output);
}
extern "C" void __fastcall DebugBoundary(void* self){
    event(1,self);if(mode&1){put(0x214,0x12345678);arena[0x218]=0;}
}
extern "C" void __fastcall BoxBoundary(const void* window,void* result){
    event(2,0,(unsigned)window);memcpy(result,window,16);
    if(mode&1){put(12,0x24000600);put(0x200,777);}
}
extern "C" void __fastcall StateBoundary(void* self,void*,const unsigned char* desc,bool update,bool force){
    event(3,self,update,force);
    // Copy only defined descriptor fields; stack padding has no value in C++.
    fwrite(desc,1,0x55,output);fwrite(desc+0x58,1,17,output);
    if(mode&2){memcpy(arena+0x14,desc,0x55);memcpy(arena+0x14+0x58,desc+0x58,17);put(12,0x24000600);}
}
static unsigned resolve(unsigned address){
    switch(address){
    case 0xb31160:return (unsigned)DebugBoundary;
    case 0x9e1e70:return (unsigned)BoxBoundary;
    case 0xa0be80:return (unsigned)Flags;
    case 0xa0be90:return (unsigned)Horizontal;
    case 0xa0bea0:return (unsigned)Vertical;
    case 0xa14440:return (unsigned)Normalize;
    case 0xb30b50:return (unsigned)StateBoundary;
    case 0x1399d44:return (unsigned)&ZStart;
    case 0x1238020:return (unsigned)&retailConstants[0];
    case 0x1238e00:return (unsigned)&retailConstants[1];
    case 0x128f608:return (unsigned)&retailConstants[2];
    }
    throw 1;
}
int main(int argc,char** argv){
    if(argc!=4||ZStart!=retailZStart)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,0x1000,0x3000,0x40);if(!code)return 6;
    memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned offset=retailFixups[i][0],target=resolve(retailFixups[i][1]);
        if(retailFixups[i][2])target-=(unsigned)(code+offset+4);
        *(unsigned*)(code+offset)=target;
    }
    typedef void (__fastcall *ProcessFunction)(void*,void*,bool);
    ProcessFunction process=argv[3][0]=='r'?(ProcessFunction)code:ProcessCamera;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;
        mode=p[0];_clearfp();_control87(p[2]?_PC_64:_PC_53,_MCW_PC);
        process(arena,0,p[1]!=0);event(4,arena);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_ENGINE_CAMERA_PROCESS_PASS cases=%u\n",cases);return 0;
}
