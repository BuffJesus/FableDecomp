#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_resource_directories_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall GetMesh(void*);
extern "C" void* __fastcall GetCache(void*);
extern "C" void* __fastcall GetWriteCache(void*);
static unsigned char* arena;static FILE* output;static unsigned mode,status;
static unsigned short path[260],text[260],suffix[32];
static void copyText(unsigned short* out,const unsigned short* in,unsigned size){memset(out,0,size*2);for(unsigned i=0;i<size;++i){out[i]=in[i];if(!in[i])return;}exit(10);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);fwrite(arena,1,256,output);fwrite(text,2,260,output);fwrite(suffix,2,32,output);}
static long __stdcall Folder(void* window,int folder,void* token,unsigned flags,unsigned short* out){if(window||folder!=26||token||flags)exit(11);event(1,26,status);memcpy(out,path,520);if(mode==1)*(unsigned*)(arena+0x10)=0x12345678;return status;}
extern "C" long (__stdcall *FolderAPI)(void*,int,void*,unsigned,unsigned short*)=Folder;
extern "C" void* __fastcall ConstructWide(void* self,void*,const unsigned short* value){copyText(text,value,260);event(2,self==arena?1:2);*(unsigned*)self=(unsigned)(arena+(self==arena?0xd00:0xd20));if(mode==1)*(unsigned*)(arena+0x14)=0x87654321;return self;}
extern "C" void* __fastcall AppendWide(void* result,void* left,const unsigned short* value){if(result!=arena)exit(12);copyText(suffix,value,32);event(3,*(unsigned*)left);*(unsigned*)result=(unsigned)(arena+0xd40);if(mode==2)*(unsigned*)left=(unsigned)(arena+0xd80);return result;}
extern "C" void __fastcall DestroyWide(void* self){if(self==arena)exit(13);event(4,*(unsigned*)self);*(unsigned*)self=0;if(mode==3)*(unsigned*)arena=(unsigned)(arena+0xd90);}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[4],cases=0;while(fread(p,4,4,input)==4){if(fread(path,1,520,input)!=520)return 5;memset(arena,p[0],256);mode=p[2];status=p[3];memset(text,0,520);memset(suffix,0,64);void* result=p[1]==0?GetMesh(arena):p[1]==1?GetCache(arena):GetWriteCache(arena);if(result!=arena)return 14;event(9);++cases;}
 fclose(input);fclose(output);printf("NATIVE_ENGINE_RESOURCE_DIRECTORIES_PASS cases=%u\n",cases);return 0;}
