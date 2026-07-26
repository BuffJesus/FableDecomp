#include "fable_boot.h"
#include "fable_filesystem.h"
#include "fable_gfmain.h"
#include "fable_system.h"

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

fable_u8* g_FableMicroThreadStackTop_013B7C84 = 0;
fable_u8* g_FableMicroThreadStackBottom_013B7C88 = 0;

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;
fable_i32 g_CCharStringInstanceCount_013BD800 = 0;
fable_u32 g_FableConsoleVariablesBoundaryCalls = 0;

namespace
{
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
    FableInstanceHandle /* instance */,
    char* commandLine,
    int /* showCommand */)
{
    return FableRunGFMainPhase1(commandLine);
}
