#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_file_installer.h"
#include "fable_retail_banks.h"
#include "installer_literals.h"
typedef void* HANDLE;
typedef void CRITICAL_SECTION;
extern "C" {
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(void*);
__declspec(dllimport) unsigned long __stdcall WaitForSingleObject(HANDLE,unsigned long);
__declspec(dllimport) int __stdcall SetEvent(HANDLE);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE);
}
enum { WAIT_TIMEOUT=258, WAIT_OBJECT_0=0 };

// Fixture-only vtable identity; virtual installer behavior is not exercised.
unsigned char g_FileInstallerVTable_0129B43C;
char g_FableEmptyNarrowString_0129AAF4[1] = {0};
static void* blocks[5];
static unsigned long sizes[5];
static unsigned allocations, threads;
static void* owner;
void* FABLE_CDECL FableInstallerContainerAllocate(unsigned long size)
{
    if (allocations >= 5) abort();
    sizes[allocations] = size;
    return blocks[allocations++] = malloc(size);
}
void FABLE_CDECL FableInstallerWorker_009C5870(void*) { abort(); }
void FABLE_CDECL FableInstallerDecompressionWorker_009C4420(void*) { abort(); }
extern "C" unsigned long __cdecl ThreadRecording(void(__cdecl* entry)(void*), unsigned stack, void* argument)
{
    if (threads >= 2 || stack != 0x3000 || argument != owner ||
        entry != (threads ? FableInstallerDecompressionWorker_009C4420 : FableInstallerWorker_009C5870)) abort();
    ++threads;
    return 0xffffffff;
}
int main()
{
    const unsigned long expected[5] = {16,16,184,20,24};
    const long initialStrings = g_CCharStringInstanceCount_013BD800;
    for (unsigned cycle = 0; cycle != 32; ++cycle)
    {
        unsigned long storage[0xa0/4];
        memset(storage, 0xa5, sizeof(storage));
        allocations = threads = 0;
        owner = storage;
        ::new (storage) CFileInstaller;
        if (allocations != 5 || threads != 2 || storage[0] != (unsigned long)&g_FileInstallerVTable_0129B43C) return 1;
        for (unsigned i=0; i!=5; ++i) if (!blocks[i] || sizes[i]!=expected[i]) return 2;
        for (i=0; i!=3; ++i) if (((void**)blocks[i])[0]!=blocks[i] || ((void**)blocks[i])[1]!=blocks[i]) return 3;
        CBasicRetailBankAliasMap& map = *(CBasicRetailBankAliasMap*)((unsigned char*)storage+0x94);
        if (map.size_ != 1 || map.head_ != blocks[4] || !map.head_->parent_) return 4;
        CBasicRetailBankAliasMap::CNode* node = map.head_->parent_;
        if (node->parent_ != map.head_ || node->left_ || node->right_ || map.head_->left_ != node || map.head_->right_ != node) return 5;
        if (strcmp((const char*)node->value_.first,g_FileInstallerCompressedExtension_0129B448) ||
            strcmp((const char*)node->value_.second,g_FileInstallerPlainExtension_0129B440)) return 6;
        if (g_CCharStringInstanceCount_013BD800 != initialStrings+2) return 7;
        CRITICAL_SECTION* lock=(CRITICAL_SECTION*)((unsigned char*)storage+4);
        EnterCriticalSection(lock); EnterCriticalSection(lock); LeaveCriticalSection(lock); LeaveCriticalSection(lock);
        for (i=0; i!=4; ++i)
        {
            HANDLE event=(HANDLE)storage[0x84/4+i];
            if (!event || WaitForSingleObject(event,0)!=WAIT_TIMEOUT || !SetEvent(event) ||
                WaitForSingleObject(event,0)!=WAIT_OBJECT_0 || WaitForSingleObject(event,0)!=WAIT_TIMEOUT || !CloseHandle(event)) return 8;
        }
        // Explicit fixture cleanup; the installer destructor remains unrecovered.
        map.clear();
        if (g_CCharStringInstanceCount_013BD800 != initialStrings || map.size_ || map.head_->parent_) return 9;
        for(i=0;i!=5;++i) free(blocks[i]);
        DeleteCriticalSection(lock);
    }
    puts("FILE_INSTALLER_CTOR_OS_OK cycles=32 events=128 worker_launches=0");
    return 0;
}
