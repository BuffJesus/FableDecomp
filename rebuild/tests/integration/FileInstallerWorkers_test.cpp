#include <stdio.h>
#include <stdlib.h>
#include "fable_file_installer.h"
void FABLE_CDECL FableInstallerWorker_009C5870(void*);
void FABLE_CDECL FableInstallerDecompressionWorker_009C4420(void*);
static unsigned stage,mode,returnValue;
static int expectedPriority;
static void* expectedOwner;
static void* expectedHandle;
static void check(bool ok) { if(!ok) abort(); }
static void* __stdcall GetThread()
{
    check(stage==0);stage=1;return expectedHandle;
}
static int __stdcall Priority(void* handle,int priority)
{
    check(stage==1 && handle==expectedHandle && priority==expectedPriority);
    stage=2;return returnValue;
}
extern "C" {
void*(__stdcall* GetSlot)()=GetThread;
int(__stdcall* PrioritySlot)(void*,int)=Priority;
}
void CFileInstaller::ThreadMain() { check(stage==2 && mode==0 && this==expectedOwner);stage=3; }
void CFileInstaller::DecompressionThreadMain() { check(stage==2 && mode==1 && this==expectedOwner);stage=3; }
int main()
{
    const unsigned values[]={0,1,0xffffffff};
    unsigned cases=0;
    for(mode=0;mode!=2;++mode)for(unsigned h=0;h!=3;++h)for(unsigned r=0;r!=3;++r)for(unsigned p=0;p!=3;++p)
    {
        stage=0;returnValue=values[r];expectedHandle=(void*)values[h];expectedOwner=(void*)values[p];
        expectedPriority=mode?0:1;
        if(mode)FableInstallerDecompressionWorker_009C4420(expectedOwner);
        else FableInstallerWorker_009C5870(expectedOwner);
        check(stage==3);++cases;
    }
    printf("FILE_INSTALLER_WORKERS_OK cases=%u actual_threads=0\n",cases);
    return 0;
}
