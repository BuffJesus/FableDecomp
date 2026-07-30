#include <stdio.h>
#include <string.h>

#include "fable_retail_banks.h"

extern "C" fable_u8 g_FableOpenIniMemoryFileVTable_0122E0FC = 0;
extern "C" wchar_t g_FableOpenIniEmptyWide_0122D70C[] = L"";
extern "C" char g_FableOpenIniEmptyChar_0129AAF4[] = "";
extern "C" char g_FableOpenIniRetailHeaderDirectory_0129B030[] =
    "RetailHeaderDirectory";
extern "C" char g_FableOpenIniDevHeaderDirectory_0129B048[] =
    "DevHeaderDirectory";
extern "C" char g_FableOpenIniEndRetailFile_0129B05C[] =
    "EndRetailFile";
extern "C" char g_FableOpenIniBeginRetailFile_0129B06C[] =
    "BeginRetailFile";

namespace
{
    fable_u32 g_directoryMarker;
    unsigned int g_treeDestroyCalls;
    unsigned int g_baseConstructCalls;
    unsigned int g_baseDestroyCalls;
    unsigned int g_defaultConstructCalls;
    unsigned int g_memoryFileOpenCalls;
    unsigned int g_memoryFileCloseCalls;
    unsigned int g_fileStreamConstructCalls;
    unsigned int g_fileStreamDestroyCalls;
    unsigned int g_wideCopyCalls;
    unsigned int g_wideAssignCalls;
    unsigned int g_wideDestroyCalls;
    unsigned int g_splitPathCalls;

    void Reset()
    {
        g_directoryMarker = 0x13572468;
        g_treeDestroyCalls = 0;
        g_baseConstructCalls = 0;
        g_baseDestroyCalls = 0;
        g_defaultConstructCalls = 0;
        g_memoryFileOpenCalls = 0;
        g_memoryFileCloseCalls = 0;
        g_fileStreamConstructCalls = 0;
        g_fileStreamDestroyCalls = 0;
        g_wideCopyCalls = 0;
        g_wideAssignCalls = 0;
        g_wideDestroyCalls = 0;
        g_splitPathCalls = 0;
    }

    bool Run(bool populatedMap)
    {
        Reset();
        fable_u8 managerBytes[sizeof(CBankFileManager)];
        fable_u8 pathnameBytes[sizeof(CWideString)];
        fable_u8 head[0x10];
        memset(managerBytes, 0, sizeof(managerBytes));
        memset(pathnameBytes, 0, sizeof(pathnameBytes));
        memset(head, 0, sizeof(head));

        CBankFileManager* manager =
            reinterpret_cast<CBankFileManager*>(managerBytes);
        if (populatedMap)
        {
            *reinterpret_cast<void**>(managerBytes + 4) = head;
            *reinterpret_cast<fable_u32*>(managerBytes + 8) = 3;
            *reinterpret_cast<void**>(head + 4) =
                reinterpret_cast<void*>(0x24681357);
        }

        const CWideString& pathname =
            *reinterpret_cast<const CWideString*>(pathnameBytes);
        manager->OpenIniFile(pathname);

        return
            g_treeDestroyCalls == (populatedMap ? 1U : 0U) &&
            *reinterpret_cast<fable_u32*>(managerBytes + 8) == 0 &&
            (!populatedMap ||
                (*reinterpret_cast<void**>(head + 4) == 0 &&
                 *reinterpret_cast<void**>(head + 8) == head &&
                 *reinterpret_cast<void**>(head + 0x0C) == head)) &&
            manager->basePathStorage_ ==
                reinterpret_cast<void*>(g_directoryMarker) &&
            g_baseConstructCalls == 1 &&
            g_baseDestroyCalls == 1 &&
            g_defaultConstructCalls == 5 &&
            g_memoryFileOpenCalls == 1 &&
            g_memoryFileCloseCalls == 1 &&
            g_fileStreamConstructCalls == 1 &&
            g_fileStreamDestroyCalls == 1 &&
            g_wideCopyCalls == 1 &&
            g_wideAssignCalls == 1 &&
            g_wideDestroyCalls == 6 &&
            g_splitPathCalls == 1;
    }
}

extern "C" __declspec(naked) void
FableOpenIniDestroyBankTree_009A8170()
{
    __asm
    {
        inc dword ptr [g_treeDestroyCalls]
        ret 4
    }
}

extern "C" void FableOpenIniFileBaseConstruct_009A8170()
{
    ++g_baseConstructCalls;
}

extern "C" void FableOpenIniFileBaseDestroy_009A8170()
{
    ++g_baseDestroyCalls;
}

extern "C" __declspec(naked) void
FableOpenIniCharDefaultConstruct_009A8170()
{
    __asm
    {
        inc dword ptr [g_defaultConstructCalls]
        mov dword ptr [ecx], 0
        ret
    }
}

extern "C" __declspec(naked) void
FableOpenIniMemoryFileOpen_009A8170()
{
    __asm
    {
        inc dword ptr [g_memoryFileOpenCalls]
        ret 0Ch
    }
}

extern "C" __declspec(naked) void
FableOpenIniFileStreamConstruct_009A8170()
{
    __asm
    {
        inc dword ptr [g_fileStreamConstructCalls]
        mov dword ptr [ecx + 4], 0
        mov dword ptr [ecx + 8], 0
        ret 8
    }
}

extern "C" __declspec(naked) void
FableOpenIniWideCopyConstruct_009A8170()
{
    __asm
    {
        inc dword ptr [g_wideCopyCalls]
        mov eax, dword ptr [esp + 4]
        mov eax, dword ptr [eax]
        mov dword ptr [ecx], eax
        ret 4
    }
}

extern "C" __declspec(naked) void
FableOpenIniSplitPath_009A8170()
{
    __asm
    {
        inc dword ptr [g_splitPathCalls]
        mov eax, dword ptr [esp + 4]
        mov edx, dword ptr [g_directoryMarker]
        mov dword ptr [eax], edx
        ret 0Ch
    }
}

extern "C" __declspec(naked) void
FableOpenIniWideAssign_009A8170()
{
    __asm
    {
        inc dword ptr [g_wideAssignCalls]
        mov eax, dword ptr [esp + 4]
        mov eax, dword ptr [eax]
        mov dword ptr [ecx], eax
        ret 4
    }
}

extern "C" void FableOpenIniWideDestroy_009A8170()
{
    ++g_wideDestroyCalls;
}

extern "C" void FableOpenIniFileStreamDestroy_009A8170()
{
    ++g_fileStreamDestroyCalls;
}

extern "C" void FableOpenIniMemoryFileClose_009A8170()
{
    ++g_memoryFileCloseCalls;
}

#define FABLE_UNUSED_OPEN_INI_STUB(name) \
    extern "C" void name() {}

FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniMemoryStreamDestroy_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniSetPosition_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniReadOverflow_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniReadLine_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniReadUntilCharacter_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniReadUntilWhitespace_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniMemoryStreamConstruct_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniWideAssignLiteral_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniWideAdd_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniCharToWide_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniCharCopyConstruct_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniCharDestroy_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniCharLiteralConstruct_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniLineDestroy_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniReadQuotedString_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniBankInfoIndex_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniFree_009A8170)
FABLE_UNUSED_OPEN_INI_STUB(FableOpenIniStringCompare_009A8170)

#undef FABLE_UNUSED_OPEN_INI_STUB

int main()
{
    if (!Run(false) || !Run(true))
    {
        printf(
            "FABLETLC_OPEN_INI_FILE FAIL tree=%u base=%u/%u "
            "default=%u memory=%u/%u stream=%u/%u copy=%u "
            "assign=%u destroy=%u split=%u\n",
            g_treeDestroyCalls,
            g_baseConstructCalls,
            g_baseDestroyCalls,
            g_defaultConstructCalls,
            g_memoryFileOpenCalls,
            g_memoryFileCloseCalls,
            g_fileStreamConstructCalls,
            g_fileStreamDestroyCalls,
            g_wideCopyCalls,
            g_wideAssignCalls,
            g_wideDestroyCalls,
            g_splitPathCalls);
        return 1;
    }

    printf("FABLETLC_OPEN_INI_FILE PASS\n");
    return 0;
}
