// Standalone bindings for the data addresses used by the retail bank-open
// ring.  The production Fable image supplies these through its IAT/data
// section; the module uses ordinary storage so the link remains runnable in
// a host process and can replace the Win32 callbacks before opening a bank.

#include <cstdlib>
#include <cstdio>

#include "rebuild_abi.h"

extern "C" int __stdcall FableOabDllEntry(void*, unsigned long, void*)
{
    return 1;
}

extern "C" bool FABLE_FASTCALL FableHostDiskOpen(
    void*, void*, const void*, unsigned long, unsigned long);
extern "C" long FABLE_FASTCALL FableHostDiskGetPosition(void*);
extern "C" unsigned long FABLE_FASTCALL FableHostDiskGetLength(void*);
extern "C" void FABLE_FASTCALL FableHostDiskReadNext(
    void*, void*, void*, long*);
extern "C" void FABLE_FASTCALL FableHostDiskRead(
    void*, void*, void*, long);

extern "C" void* g_FableOpenRetailDiskVTable_0122D06C[11] = {
    0,
    (void*)FableHostDiskOpen,
    0, 0, 0, 0, 0,
    (void*)FableHostDiskGetPosition,
    (void*)FableHostDiskReadNext,
    (void*)FableHostDiskGetLength,
    (void*)FableHostDiskRead,
};
extern "C" unsigned long g_FableOpenRetailMagic_0129B07C = 0x42474942UL;

extern "C" unsigned char g_CBaseVTable_009AC4D0 = 0;
extern "C" wchar_t g_FableEmptyWideString_0129A8E0[1] = { 0 };
extern "C" void (*g_FableFreeThunkTarget_00440154)(void*) = &std::free;
extern "C" unsigned long (*g_FableGetFileAttributesW_0143FDBC)(const wchar_t*) = 0;
extern "C" unsigned char g_FableRetailBankVTable_0129B010 = 0;

typedef void* (__stdcall *FableCreateFileWHostProc)(
    const wchar_t*, unsigned long, unsigned long, void*, unsigned long,
    unsigned long, void*);
typedef unsigned long (__stdcall *FableGetFileSizeHostProc)(
    void*, unsigned long*);

static void* __stdcall FableHostCreateFileW(
    const wchar_t* path, unsigned long, unsigned long, void*, unsigned long,
    unsigned long, void*)
{
    FILE* file = _wfopen(path ? path : L"", L"rb");
    return file ? (void*)file : (void*)-1;
}

static unsigned long __stdcall FableHostGetFileSize(
    void* handle, unsigned long*)
{
    FILE* file = (FILE*)handle;
    if (!file) return 0xFFFFFFFFUL;
    long old = ftell(file);
    if (old < 0) return 0xFFFFFFFFUL;
    if (fseek(file, 0, SEEK_END) != 0) return 0xFFFFFFFFUL;
    long end = ftell(file);
    fseek(file, old, SEEK_SET);
    return end < 0 ? 0xFFFFFFFFUL : (unsigned long)end;
}

extern "C" FableCreateFileWHostProc g_FableThreadedCreateFileW_0143FE2C =
    &FableHostCreateFileW;
extern "C" char g_FableThreadedDDrive_0129A15C[3] = { 'D', ':', 0 };
extern "C" unsigned char g_FableThreadedEmptyChar_0122D70E = 0;
extern "C" FableGetFileSizeHostProc g_FableThreadedGetFileSize_0143FDF0 =
    &FableHostGetFileSize;
extern "C" unsigned long g_FableThreadedPhysicalSortKey_013BC9EC = 0;

extern "C" void* __cdecl operator_new_impl(unsigned int size)
{
    return std::malloc(size);
}

extern "C" void __cdecl operator_delete(void* p)
{
    std::free(p);
}
