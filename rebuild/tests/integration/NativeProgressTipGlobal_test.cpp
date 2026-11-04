#include <stdio.h>
#include <stdlib.h>
#include "fable_string.h"
extern CWideString GResetProgressTipText,GResetProgressTipTitleText;
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
#pragma comment(linker,"/alternatename:?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z=@UnexpectedAllocation@16")
extern "C" void* __fastcall UnexpectedAllocation(void*,void*,const unsigned short*,long){exit(10);return 0;}
static unsigned char* arena;static FILE* output;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned arg=0){unsigned h[]={kind,arg};fwrite(h,4,2,output);fwrite(&GResetProgressTipText,4,1,output);fwrite(&GResetProgressTipTitleText,4,1,output);fwrite(&g_CWideStringInstanceCount_013BCA20,4,1,output);fwrite(arena,1,4096,output);}
extern "C" void __cdecl TipFree(void* p){event(2,(unsigned)p);}
void operator delete(void* p){event(3,(unsigned)p);}
static void finalObservation(){event(9);fclose(output);}
#pragma init_seg(lib)
struct FinalObserver{FinalObserver(){atexit(finalObservation);}};
static FinalObserver observer;
int main(int argc,char** argv){
 if(argc!=3)return 2;output=fopen(argv[1],"wb");if(!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;event(0);
 unsigned mode=atoi(argv[2]);if(mode){
  *(unsigned*)&GResetProgressTipText=(unsigned)(arena+0x100);*(unsigned*)&GResetProgressTipTitleText=(unsigned)(arena+(mode==2?0x100:0x120));
  for(unsigned i=0;i<2;++i){unsigned o=0x100+i*0x20;word(o)=(unsigned)(arena+0x400+i*0x40);word(o+4)=word(o)+4;word(o+8)=word(o)+16;word(o+12)=mode==2?2:mode==3?0:1;}
 }
 event(1);printf("NATIVE_PROGRESS_TIP_GLOBAL_PASS mode=%u\n",mode);return 0;
}
