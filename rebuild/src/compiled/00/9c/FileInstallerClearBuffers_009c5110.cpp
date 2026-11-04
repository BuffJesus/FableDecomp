// Native cleanup provider: retail parity and behavior evidence enforced by permanent gates.
#include "fable_file_installer.h"
#include "fable_critical_section_lock.h"
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void*);
void CFileInstaller::ClearBuffers()
{
    unsigned char* bytes=reinterpret_cast<unsigned char*>(this);
    CCriticalSectionLock lock(*reinterpret_cast<_RTL_CRITICAL_SECTION*>(bytes+4),false);
    reinterpret_cast<CFileInstallerBufferArray*>(bytes+0x1c)->DeallocateMemory();
    typedef void*(__stdcall* FreeBuffer)(void*);
    FreeBuffer release=GlobalFree;
    void** read=reinterpret_cast<void**>(bytes+0x28);
    void** write=reinterpret_cast<void**>(bytes+0x2c);
    if(*read) { release(*read); *read=0; }
    if(*write) { release(*write); *write=0; }
}
