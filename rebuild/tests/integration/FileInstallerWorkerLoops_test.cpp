#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_file_installer.h"
#include "fable_critical_section_lock.h"
#include "engine/CCompressorZlib.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;
static FILE* output;
static unsigned mode,mutation,limit,cycles,depth;
static unsigned long& word(unsigned offset) {return *reinterpret_cast<unsigned long*>(arena+offset);}
static void event(unsigned kind,unsigned a=0,unsigned b=0) {
    unsigned header[]={kind,a,b,depth};
    if(fwrite(header,4,4,output)!=4 || fwrite(arena,1,4096,output)!=4096)abort();
}
static void completedCycle(){if(++cycles>=limit)arena[0x7a]=1;}
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& section,bool tryOnly)
{
    if(&section!=reinterpret_cast<_RTL_CRITICAL_SECTION*>(arena+4)||tryOnly)abort();
    event(1,(unsigned)&section,tryOnly);CriticalSection=&section;Locked=true;++depth;
}
void CCriticalSectionLock::Release(){event(2,Locked);if(Locked){--depth;Locked=false;}
    if(mode==1 && mutation && !arena[0x7a]) {word(0x54)=(unsigned)(arena+0x500);word(0x30)=0;}
}
void CCriticalSectionLock::Lock(){event(3,Locked);if(!Locked){++depth;Locked=true;}}
CCriticalSectionLock::~CCriticalSectionLock(){event(4,Locked);if(Locked)--depth;}
static unsigned long __stdcall Wait(void* handle,unsigned long timeout,int alertable){
    if(alertable!=1 || depth)abort();event(5,(unsigned)handle,timeout);
    if(mode==1 || timeout==100)completedCycle();
    else if(handle==(void*)word(0x90))word(0x7c)=0;
    else {
        word(0x300)=(unsigned)(arena+0x340);
        word(0x58)=0;word(0x5c)=0;word(0x74)=(unsigned)(arena+0x700);
    }
    return 0xc0;
}
static int __stdcall Signal(void* handle){if(depth)abort();event(6,(unsigned)handle);return 1;}
extern "C" {
unsigned long(__stdcall* WaitSlot)(void*,unsigned long,int)=Wait;
int(__stdcall* SignalSlot)(void*)=Signal;
}
void CFileInstaller::ProcessActiveFileForDecompression(CActiveFile& active){
    if(this!=(CFileInstaller*)arena||depth)abort();event(7,(unsigned)&active);completedCycle();
}
void CFileInstaller::PostFileReadCompletion(CActiveRead* active){
    if(this!=(CFileInstaller*)arena||depth!=2)abort();event(8,(unsigned)active);
    if(mutation)word(0x74)=0x12345678;
}
void CFileInstaller::ProcessActiveFileForWrite(CActiveFile& active){
    if(this!=(CFileInstaller*)arena||depth!=1)abort();event(9,(unsigned)&active);
}
void CFileInstaller::ProcessReads(){if(this!=(CFileInstaller*)arena||depth!=1)abort();event(10);}
void CFileInstaller::ProcessAlways(){if(this!=(CFileInstaller*)arena||depth!=1)abort();event(11);
    if(mutation){arena[0x60]=!arena[0x60];word(0x34)=cycles;word(0x68)=3;}
}
int main(int argc,char** argv){
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[3],cases=0;
    while(fread(params,4,3,input)==3){
        mode=params[0];mutation=params[1];limit=params[2];depth=cycles=0;
        if(fread(arena,1,4096,input)!=4096)return 5;
        if(mode)((CFileInstaller*)arena)->DecompressionThreadMain();else((CFileInstaller*)arena)->ThreadMain();
        if(depth)return 6;event(12);++cases;
    }
    fclose(input);fclose(output);VirtualFree(arena,0,0x8000);
    printf("INSTALLER_WORKER_LOOPS_OK cases=%u\n",cases);return 0;
}
