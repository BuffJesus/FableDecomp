#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
__declspec(dllimport) void* __stdcall GlobalAlloc(unsigned,unsigned long);
}
class InstallerBufferArrayView { public: void DeallocateMemory(); };
class InstallerClearBuffersView { public: void ClearBuffers(); };
static unsigned char* owner;
static unsigned calls,mode;
void InstallerBufferArrayView::DeallocateMemory()
{
    if ((unsigned char*)this != owner+0x1c || ++calls!=1 || *(long*)(owner+12)!=1) abort();
    if(mode) {
        if(!*(void**)(owner+0x28)) { *(void**)(owner+0x28)=GlobalAlloc(0,32); if(!*(void**)(owner+0x28))abort(); }
        if(!*(void**)(owner+0x2c)) { *(void**)(owner+0x2c)=GlobalAlloc(0,48); if(!*(void**)(owner+0x2c))abort(); }
    }
}
int main()
{
    unsigned cases=0;
    for(unsigned cycle=0;cycle!=8;++cycle)for(mode=0;mode!=2;++mode)for(unsigned mask=0;mask!=4;++mask)
    {
        unsigned long storage[40];owner=(unsigned char*)storage;memset(storage,0xa5,sizeof(storage));calls=0;
        InitializeCriticalSection(owner+4);
        storage[0x28/4]=(unsigned long)((mask&1)?GlobalAlloc(0,16):0);
        storage[0x2c/4]=(unsigned long)((mask&2)?GlobalAlloc(0,24):0);
        if(((mask&1)&&!storage[10])||((mask&2)&&!storage[11]))abort();
        ((InstallerClearBuffersView*)owner)->ClearBuffers();
        if(calls!=1||storage[10]||storage[11]||*(long*)(owner+12)!=0||storage[0]!=0xa5a5a5a5||storage[12]!=0xa5a5a5a5)return 1;
        DeleteCriticalSection(owner+4);++cases;
    }
    printf("FILE_INSTALLER_CLEAR_BUFFERS_OS_OK cases=%u\n",cases);
    return 0;
}
