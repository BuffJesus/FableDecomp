#pragma once

#include "rebuild_abi.h"
#include "fable_file_installer_base.h"
#include <stddef.h>

void* FABLE_CDECL FableFileInstallerAllocate(fable_u32 size);

// Shipping installer BufferChunks at +0x1C: three pointers to 8-byte entries.
// Range destruction reads the owned allocation at +0 and preserves +4.
// The donor's name for the second word has not been established.
struct InstallerBufferChunk
{
    void* allocation;
    fable_u32 unknown04;
};
FABLE_STATIC_ASSERT(sizeof(InstallerBufferChunk) == 8);

void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk*,InstallerBufferChunk*,void*);

class CFileInstallerBufferArray
{
public:
    InstallerBufferChunk *first, *last, *capacity;
    InstallerBufferChunk* RemoveRange(InstallerBufferChunk*,InstallerBufferChunk*); // 009C7150
    void DeallocateMemory(); // 009C76D0
};
FABLE_STATIC_ASSERT(sizeof(CFileInstallerBufferArray) == 12);

// Retail RTTI .?AVCFileInstaller@@: a class.
class CFileInstaller : public CTBaseSingleton<CFileInstaller>
{
public:
    class CFileInfo;
    class CActiveFile;
    class CActiveRead;
    unsigned long CriticalSection[6]; // +04, retail Win32 storage
    fable_u8 unknown1c[0x5f];
    // FableWin PDB confirms both qualifiers; retail Unpause preserves each read.
    volatile bool Running; // +7B
    volatile unsigned long Paused; // +7C, pause reference count
    fable_u8 unknown80;
    fable_u8 basicInstallStartupFlag;
    fable_u8 unknown82[2];
    void* Event; // +84
    fable_u8 unknown88[4];
    void* PauseStartEvent; // +8C
    void* PauseFinishEvent; // +90
    fable_u8 unknown94[0x0c];

protected:
    static bool DirtyDisc; // 013CA814, FableWin protected static
public:
    CFileInstaller();
    virtual ~CFileInstaller();
    void ClearBuffers(); // retail 009C5110
    void ThreadMain(); // retail 009C5690
    void DecompressionThreadMain(); // retail 009C4230
    void ProcessActiveFileForDecompression(CActiveFile&); // 009C3C20
    void ProcessActiveFileForWrite(CActiveFile&); // 009C3FB0
    void PostFileReadCompletion(CActiveRead*); // 009C4120
    void ProcessReads(); // 009C53A0
    void Pause(); // retail 009C31E0
    void Unpause(); // retail 009C3250
    void ProcessAlways(); // 009C4F20

    __forceinline static void* operator new(size_t size)
    {
        return FableFileInstallerAllocate(size);
    }
};

FABLE_STATIC_ASSERT(sizeof(CFileInstaller) == 0xA0);
FABLE_STATIC_ASSERT(offsetof(CFileInstaller, CriticalSection) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CFileInstaller, Running) == 0x7B);
FABLE_STATIC_ASSERT(offsetof(CFileInstaller, Paused) == 0x7C);
FABLE_STATIC_ASSERT(offsetof(CFileInstaller, basicInstallStartupFlag) == 0x81);
FABLE_STATIC_ASSERT(offsetof(CFileInstaller, Event) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CFileInstaller, PauseStartEvent) == 0x8C);
FABLE_STATIC_ASSERT(offsetof(CFileInstaller, PauseFinishEvent) == 0x90);

void FABLE_FASTCALL FableDestroyFileInstaller_00403630(void* object);

struct CCountedFileInstaller
{
    CFileInstaller* object;
    void* reference;

    void operator=(CFileInstaller* value);
    void Reset();
};

FABLE_STATIC_ASSERT(sizeof(CCountedFileInstaller) == 0x08);

// The singleton is owned through the engine's counted-pointer wrapper. Keeping
// the wrapper visible here documents why the retail storage occupies two
// pointers even though Get() returns only the CFileInstaller object.
extern CCountedFileInstaller g_CFileInstallerSingleton_013CA818;

struct CFileInstallerSingleton
{
    static CFileInstaller& FABLE_FASTCALL Get();
};
