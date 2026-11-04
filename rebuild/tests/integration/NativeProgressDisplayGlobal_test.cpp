#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_boot.h"
#include "fable_progress_display.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void __fastcall DeleteDisplay(void*);
static void event(unsigned kind,unsigned arg=0,unsigned flag=0){unsigned h[]={kind,arg,flag};unsigned char copy[4096];memcpy(copy,arena,4096);unsigned& fn=*(unsigned*)(copy+0x104);if(fn==(unsigned)&DeleteDisplay)fn=0x24009200;fwrite(h,4,3,output);fwrite(&NProgressDisplay::PProgressDisplay,4,2,output);fwrite(copy,1,4096,output);}
static void __fastcall DeleteDisplay(void* p){event(3,(unsigned)p);if(mode==5){NProgressDisplay::PProgressDisplay.Data=0;NProgressDisplay::PProgressDisplay.Info=(CCPPointerInfo*)(arena+0x120);}}
void operator delete(void* p){event(4,(unsigned)p);}
static void __fastcall StopBoundary(void* p,void*,bool a,bool b){if(p!=arena+0x40||!a||b)exit(11);event(2,(unsigned)p,(a?1:0)|(b?2:0));++word(0x44);}
static void finalObservation(){event(9);fclose(output);}
#pragma init_seg(lib)
struct FinalObserver{FinalObserver(){atexit(finalObservation);}};
static FinalObserver observer;
int main(int argc,char** argv){
 if(argc!=3)return 2;output=fopen(argv[1],"wb");if(!output)return 3;mode=atoi(argv[2]);arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 if((void*)&g_FableProgressDisplay_013CAA38!=(void*)&NProgressDisplay::PProgressDisplay)return 12;
 word(0x8008)=(unsigned)&StopBoundary;event(0);
 if(mode){NProgressDisplay::PProgressDisplay.Data=(CAProgressDisplay*)(arena+0x40);NProgressDisplay::PProgressDisplay.Info=(CCPPointerInfo*)(arena+0x100);word(0x40)=(unsigned)(arena+0x8000);word(0x100)=mode==2?2:mode==3?0:mode==4?0xffffffff:1;word(0x104)=(unsigned)&DeleteDisplay;word(0x108)=(unsigned)(arena+0x40);}
 event(1);NProgressDisplay::StopProgress(true,false);event(5);printf("NATIVE_PROGRESS_DISPLAY_GLOBAL_PASS mode=%u\n",mode);return 0;
}
