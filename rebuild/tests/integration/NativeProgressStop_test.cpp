#include <stdio.h>
#include <stdlib.h>
#include "progress_stop_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall StopProgress(bool,bool);
extern "C" unsigned ProgressOwner[2]={0,0};
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned self=0,unsigned a=0,unsigned b=0){unsigned h[]={kind,self,a,b};fwrite(h,4,4,output);fwrite(ProgressOwner,4,2,output);fwrite(arena,1,256,output);}
extern "C" void __fastcall StopBoundary(void* self,void*,bool a,bool b){if(self!=arena+0x40)exit(10);event(1,(unsigned)self,a,b);++word(0x44);if(mode==1)ProgressOwner[0]=0;else if(mode==2){ProgressOwner[0]=(unsigned)(arena+0x80);ProgressOwner[1]=0;}}
int main(int argc,char** argv){if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8008)=(unsigned)&StopBoundary;
 unsigned p[3],cases=0;while(fread(p,4,3,input)==3){if(fread(ProgressOwner,4,2,input)!=2||fread(arena,1,256,input)!=256)return 5;mode=p[0];StopProgress(p[1]!=0,p[2]!=0);event(2);++cases;}
 fclose(input);fclose(output);printf("NATIVE_PROGRESS_STOP_PASS cases=%u\n",cases);return 0;}
