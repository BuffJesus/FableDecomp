#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader_manager_dtor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall DestroyManager(void*);
extern "C" void* __fastcall DeleteManager(void*,void*,unsigned);
extern "C" void* __fastcall DeletePixel(void*,void*,unsigned);
extern "C" unsigned char ManagerTable[],PixelTable[],ShaderTable[],PoolTable[],BaseTable[],NonCopyTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){
 unsigned h[]={kind,a,b,c};unsigned char copy[0x4000];memcpy(copy,arena,sizeof(copy));
 unsigned tables[]={(unsigned)ManagerTable,(unsigned)PixelTable,(unsigned)ShaderTable,(unsigned)PoolTable,(unsigned)BaseTable,(unsigned)NonCopyTable};
 unsigned retail[]={0x129a080,0x129a054,0x129a030,0x129a064,0x1231710,0x129a7c4};
 unsigned offsets[]={0,0x178,0x1e0,0x18,0x1c,0x28,0x34,0x40,0x4c,0x58,0x68,0x74,0x80,0x8c,0x98,0xa4,0xb0,0xbc,0xc8,0xd4,0xe0,0xec,0xf8,0x104,0x110,0x11c,0x128,0x134,0x140,0x14c};
 for(unsigned i=0;i<sizeof(offsets)/4;++i)for(unsigned j=0;j<6;++j)if(*(unsigned*)(copy+offsets[i])==tables[j]){*(unsigned*)(copy+offsets[i])=retail[j];break;}
 // These are actual callback pointers, whose identity is checked before mapping.
 for(i=0;i<3;++i){unsigned& p=*(unsigned*)(copy+0x1404+i*16);extern void __fastcall DeleteResource(void*);if(p==(unsigned)&DeleteResource)p=0x24009200;}
 fwrite(h,4,4,output);fwrite(copy,1,sizeof(copy),output);
}
extern "C" void __fastcall UninitialiseBoundary(void* self){event(1,(unsigned)self);if(mode==1)word(0x3cc)=(unsigned)(arena+0x2100);}
extern "C" void __cdecl FableManagerFree(void* p){event(2,(unsigned)p);if(mode==2)word(0x390)^=0x12345678;if(mode==3&&p==arena+0x2000)word(0x3cc)=(unsigned)(arena+0x2140);}
void operator delete(void* p){if(output)event(3,(unsigned)p);}
extern "C" void __fastcall EraseTreeBoundary(void* self,void*,void* root){event(4,(unsigned)self,(unsigned)root);if(mode==3)*(unsigned*)self=(unsigned)(arena+0x1140);}
extern "C" void __fastcall StringDtorBoundary(void* self){event(5,(unsigned)self);*(unsigned*)self=0;if(mode==1)word(0x3b8)=(unsigned)(arena+0x1200);}
extern "C" void* __fastcall DeleteValue(void* self,void*,unsigned flags){event(6,(unsigned)self,flags);if(mode==2&&self>=arena+0x1800&&self<arena+0x1900)*(unsigned*)((char*)self+20)=0;*(unsigned*)((char*)self+4)=0x76543210;return self;}
void __fastcall DeleteResource(void* self){event(7,(unsigned)self);if(mode==3)word(0x3a8)=(unsigned)(arena+0x1340);}
extern "C" void UnusedShaderManagerDtorBoundary(){exit(99);}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8000)=(unsigned)&DeleteValue;
 unsigned p[6],cases=0;while(fread(p,4,6,input)==6){if(fread(arena,1,0x4000,input)!=0x4000)return 5;mode=p[5];for(unsigned i=0;i<p[2];++i)word(0x1404+i*16)=(unsigned)&DeleteResource;unsigned result=0;unsigned flags[]={0,1,2,0xffffffff};if(p[0]>=5)result=(unsigned)DeletePixel(arena+0x1e0,0,flags[(p[0]-1)%4]);else if(p[0])result=(unsigned)DeleteManager(arena,0,flags[p[0]-1]);else DestroyManager(arena);event(8,p[0],result);++cases;}
 fclose(input);fclose(output);output=0;printf("NATIVE_SHADER_MANAGER_DTOR_PASS cases=%u\n",cases);return 0;
}
