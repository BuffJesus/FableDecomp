#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader_manager_global_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned char ManagerGlobal[],ManagerTable[],PixelTable[],ShaderTable[],PoolTable[],LayoutTable[],RangeTable[],BaseTable[],NonCopyTable[];
static unsigned char* arena;static unsigned char records[32*1032];static unsigned used;static FILE* output;
static void event(unsigned kind,unsigned arg=0){
 unsigned* h=(unsigned*)(records+used);h[0]=kind;h[1]=arg;unsigned char* copy=records+used+8;memcpy(copy,ManagerGlobal,1000);memcpy(copy+1000,arena+0x1000,24);
 unsigned tables[]={(unsigned)ManagerTable,(unsigned)PixelTable,(unsigned)ShaderTable,(unsigned)PoolTable,(unsigned)LayoutTable,(unsigned)RangeTable,(unsigned)BaseTable,(unsigned)NonCopyTable};
 unsigned retail[]={0x129a080,0x129a054,0x129a030,0x129a064,0x129a06c,0x129a04c,0x1231710,0x129a7c4};
 unsigned offsets[]={0,0x178,0x1e0,0x18,0x1c,0x28,0x34,0x40,0x4c,0x58,0x68,0x74,0x80,0x8c,0x98,0xa4,0xb0,0xbc,0xc8,0xd4,0xe0,0xec,0xf8,0x104,0x110,0x11c,0x128,0x134,0x140,0x14c};
 for(unsigned i=0;i<sizeof(offsets)/4;++i)for(unsigned j=0;j<8;++j)if(*(unsigned*)(copy+offsets[i])==tables[j]){*(unsigned*)(copy+offsets[i])=retail[j];break;}
 used+=1032;
}
extern "C" void* __cdecl FableGlobalMalloc(unsigned n){if(n!=24)exit(10);arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)exit(11);event(1,n);return arena+0x1000;}
extern "C" void __cdecl FableGlobalFree(void* p){event(4,(unsigned)p);}
extern "C" void UnusedShaderManagerGlobalBoundary(){exit(99);}
static void finalObservation(){event(5);if(fwrite(records,1,used,output)!=used)exit(12);fclose(output);}
// The observer registers before the production user-segment initializer so its
// final callback runs after the actual CRT-registered manager destructor.
#pragma init_seg(lib)
struct FinalObserver{FinalObserver(){atexit(finalObservation);}};
static FinalObserver observer;
int main(int argc,char** argv){
 if(argc!=2)return 2;output=fopen(argv[1],"wb");if(!output)return 3;event(2);
 unsigned offsets[]={0x3dc,0x3cc,0x1a0,0x194,0x158,0xc};unsigned strides[]={48,16,16,16,16,4};
 for(unsigned i=0;i<6;++i){unsigned* v=(unsigned*)(ManagerGlobal+offsets[i]);v[0]=(unsigned)(arena+0x2000+i*0x100);v[1]=v[0]+strides[i];v[2]=v[0]+2*strides[i];}
 *(unsigned*)(ManagerGlobal+0x1d4)=0xabcdef00;event(3);printf("NATIVE_SHADER_MANAGER_GLOBAL_PASS\n");return 0;
}
