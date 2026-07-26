#include "fable_boot.h"
#include "fable_async.h"
#include "fable_file_installer.h"
#include "fable_filesystem.h"
#include "fable_gfmain.h"
#include "fable_startup.h"
#include "fable_system.h"
#if defined(FABLETLC_ENABLE_VISUAL_BOOT)
#include "fable_visual_boot.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

fable_u8* g_FableMicroThreadStackTop_013B7C84 = 0;
fable_u8* g_FableMicroThreadStackBottom_013B7C88 = 0;

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;
fable_i32 g_CCharStringInstanceCount_013BD800 = 0;
fable_u32 g_FableConsoleVariablesBoundaryCalls = 0;
fable_u32 g_CBankFileAsyncFailureHandlingMode_013CA868 = 0;
bool g_FableStartupLatch_013964A8 = false;

CCountedFileInstaller g_CFileInstallerSingleton_013CA818 = {0, 0};
GFMainPhase2State g_GFMainPhase2State = {};

fable_u32 g_FableFileInstallerConstructCalls = 0;
fable_u32 g_FableFileInstallerDestroyCalls = 0;
fable_u8 g_FableLastBasicInstallStartupFlag = 0;

namespace
{
    typedef void (FABLE_FASTCALL *DestroyCountedObject)(void* object);

    struct FileInstallerReference
    {
        fable_i32 owners;
        DestroyCountedObject destroy;
        void* object;
    };

    void FABLE_FASTCALL DestroyFileInstaller(void* object)
    {
        CFileInstaller* installer =
            static_cast<CFileInstaller*>(object);
        g_FableLastBasicInstallStartupFlag =
            installer->basicInstallStartupFlag;
        ++g_FableFileInstallerDestroyCalls;
        free(installer);
    }

    CWideStringData* AllocateWideString(const wchar_t* text)
    {
        const size_t characters = wcslen(text) + 1;
        CWideStringData* data =
            static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
        if (data == 0)
            return 0;

        data->text = static_cast<wchar_t*>(
            malloc(characters * sizeof(wchar_t)));
        if (data->text == 0)
        {
            free(data);
            return 0;
        }

        memcpy(data->text, text, characters * sizeof(wchar_t));
        data->unknown04 = 0;
        data->unknown08 = 0;
        data->owners = 1;
        return data;
    }

    void ReleaseWideString(CWideStringData*& data)
    {
        if (data == 0)
            return;

        --data->owners;
        if (data->owners <= 0)
        {
            free(data->text);
            free(data);
        }
        data = 0;
    }
}

void* FABLE_CDECL FableFileInstallerAllocate(fable_u32 size)
{
    return malloc(size);
}

CFileInstaller::CFileInstaller()
{
    memset(this, 0, sizeof(*this));
    ++g_FableFileInstallerConstructCalls;
}

void CCountedFileInstaller::operator=(CFileInstaller* value)
{
    Reset();
    object = value;
    if (value == 0)
        return;

    FileInstallerReference* owner =
        static_cast<FileInstallerReference*>(
            malloc(sizeof(FileInstallerReference)));
    if (owner == 0)
    {
        DestroyFileInstaller(value);
        object = 0;
        return;
    }

    owner->owners = 1;
    owner->destroy = DestroyFileInstaller;
    owner->object = value;
    reference = owner;
}

void CCountedFileInstaller::Reset()
{
    FileInstallerReference* owner =
        static_cast<FileInstallerReference*>(reference);
    if (owner != 0)
    {
        --owner->owners;
        if (owner->owners == 0)
        {
            owner->destroy(owner->object);
            free(owner);
        }
    }

    object = 0;
    reference = 0;
}

void* operator new(size_t, void* address)
{
    return address;
}

namespace MicroThread
{
    void FABLE_FASTCALL SetStack(
        fable_u8* /* stackTop */,
        fable_u32 /* stackSize */)
    {
    }
}

CWideString::CWideString(const wchar_t* text)
    : storage_(AllocateWideString(text))
{
    ++g_CWideStringInstanceCount_013BCA20;
}

CWideString::operator const wchar_t*() const
{
    static const wchar_t kEmpty[] = L"";
    return storage_ != 0 ? storage_->text : kEmpty;
}

CWideString& CWideString::operator=(const CWideString& other)
{
    if (this != &other)
    {
        ReleaseWideString(storage_);
        storage_ = other.storage_;
        if (storage_ != 0)
            ++storage_->owners;
    }
    return *this;
}

const CWideString& CWideString::operator=(const wchar_t* text)
{
    ReleaseWideString(storage_);
    storage_ = AllocateWideString(text);
    return *this;
}

CCharStringData* CCharString::AllocStringData(
    const char* text,
    long length)
{
    if (length < 0)
        length = static_cast<long>(strlen(text));

    CCharStringData* data =
        static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));
    if (data == 0)
        return 0;

    data->text = static_cast<char*>(malloc(length + 1));
    if (data->text == 0)
    {
        free(data);
        return 0;
    }

    memcpy(data->text, text, length);
    data->text[length] = '\0';
    data->unknown04 = 0;
    data->unknown08 = 0;
    data->flags0C = 0;
    data->owners = 1;
    return data;
}

void CCharString::UnassignString()
{
    if (storage_ == 0)
        return;

    --storage_->owners;
    if (storage_->owners <= 0)
    {
        free(storage_->text);
        free(storage_);
    }
    storage_ = 0;
}

void FABLE_FASTCALL FableConstructWideString(CWideString* value)
{
    new (value) CWideString;
}

void CRegionDef::ConstructStorage()
{
}

CWideString g_CAFileCurrentPath_013BCA10;

void FABLE_FASTCALL FableInitialiseConsoleVariablesBoundary()
{
    ++g_FableConsoleVariablesBoundaryCalls;
}

extern "C" long FABLE_FASTCALL GFMain(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand)
{
    const long phase1Result = FableRunGFMainPhase1(commandLine);
    if (phase1Result != 0)
        return phase1Result;

#if defined(FABLETLC_ENABLE_GFMAIN_PHASE2)
    const long phase2Result = FableRunGFMainPhase2(g_GFMainPhase2State);
    if (phase2Result != 0)
        return phase2Result;

#if defined(FABLETLC_ENABLE_VISUAL_BOOT)
    FablePrepareGFInitialiseBoundary();
    if (!GFInitialise())
    {
        return 1;
    }

    return FableRunVisualBootCheckpoint(
        instance,
        commandLine,
        showCommand);
#else
    return 0;
#endif
#else
    return 0;
#endif
}
