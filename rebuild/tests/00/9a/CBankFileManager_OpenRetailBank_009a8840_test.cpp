#include <stdio.h>
#include <string.h>

#include "fable_retail_banks.h"

namespace
{
    struct DiskVTable
    {
        void* destroy;
        void* open;
    };

    fable_u8 g_disk[0x1C];
    fable_u32 g_header[2];
    bool g_pathExists;
    unsigned int g_allocations;
    unsigned int g_diskOpenCalls;
    unsigned int g_streamConstructCalls;
    unsigned int g_streamDestroyCalls;
    unsigned int g_ownerDestroyCalls;

    void Reset(bool pathExists, fable_u32 magic, fable_u32 version)
    {
        memset(g_disk, 0, sizeof(g_disk));
        g_header[0] = magic;
        g_header[1] = version;
        g_pathExists = pathExists;
        g_allocations = 0;
        g_diskOpenCalls = 0;
        g_streamConstructCalls = 0;
        g_streamDestroyCalls = 0;
        g_ownerDestroyCalls = 0;
    }

    bool Check(
        bool pathExists,
        fable_u32 magic,
        fable_u32 version,
        unsigned int expectedOpenCalls,
        unsigned int expectedOwnerDestroyCalls)
    {
        Reset(pathExists, magic, version);
        fable_u8 managerBytes[sizeof(CBankFileManager)];
        fable_u8 pathnameBytes[sizeof(CWideString)];
        memset(managerBytes, 0, sizeof(managerBytes));
        memset(pathnameBytes, 0, sizeof(pathnameBytes));
        CBankFileManager* manager =
            reinterpret_cast<CBankFileManager*>(managerBytes);
        manager->retailBankVersion_ = 7;
        const CWideString& pathname =
            *reinterpret_cast<const CWideString*>(pathnameBytes);

        manager->OpenRetailBank(pathname, true);

        if (!pathExists)
        {
            return
                g_allocations == 0 &&
                g_diskOpenCalls == 0 &&
                g_streamConstructCalls == 0 &&
                g_streamDestroyCalls == 0 &&
                g_ownerDestroyCalls == 0;
        }

        return
            g_allocations == 1 &&
            g_diskOpenCalls == expectedOpenCalls &&
            g_streamConstructCalls == 1 &&
            g_streamDestroyCalls == 1 &&
            g_ownerDestroyCalls == expectedOwnerDestroyCalls;
    }
}

extern "C" __declspec(naked) void
FableOpenRetailDiskOpenStub_009A8840()
{
    __asm
    {
        inc dword ptr [g_diskOpenCalls]
        ret 0Ch
    }
}

extern "C" DiskVTable g_FableOpenRetailDiskVTable_0122D06C =
{
    0,
    FableOpenRetailDiskOpenStub_009A8840
};
extern "C" fable_u32 g_FableOpenRetailMagic_0129B07C = 0x42474942;

extern "C" bool FableOpenRetailPathExists_009A8840()
{
    return g_pathExists;
}

extern "C" void* FableOpenRetailOperatorNew_009A8840(fable_u32 size)
{
    ++g_allocations;
    return size == sizeof(g_disk) ? g_disk : 0;
}

extern "C" void FableOpenRetailDiskBaseConstruct_009A8840()
{
}

extern "C" void FableOpenRetailCharConstruct_009A8840()
{
}

extern "C" __declspec(naked) void
FableOpenRetailDiskOwnerConstruct_009A8840()
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        mov dword ptr [ecx], eax
        mov dword ptr [ecx + 4], 0
        ret 4
    }
}

extern "C" __declspec(naked) void
FableOpenRetailFileStreamConstruct_009A8840()
{
    __asm
    {
        inc dword ptr [g_streamConstructCalls]
        mov dword ptr [ecx + 4], 0
        mov eax, offset g_header
        mov dword ptr [ecx + 0Ch], eax
        mov dword ptr [ecx + 14h], 8
        ret 8
    }
}

extern "C" void FableOpenRetailReadOverflow_009A8840()
{
}

extern "C" void FableOpenRetailFileStreamDestroy_009A8840()
{
    ++g_streamDestroyCalls;
}

extern "C" void FableOpenRetailDiskOwnerDestroy_009A8840()
{
    ++g_ownerDestroyCalls;
}

#define FABLE_UNUSED_OPEN_RETAIL_STUB(name) \
    extern "C" void name() {}

FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailBankConstruct_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailBankOwnerConstruct_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailBankListInsert_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailWideToChar_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailCharAssign_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailCharDestroy_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailDiskOwnerAssign_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailBufferAllocate_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailSetPosition_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailMemoryStreamConstruct_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailReadNullString_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailContainedBankIndex_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailMemoryStreamDestroy_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailFree_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailThreadedConstruct_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailThreadedReset_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailThreadedOpen_009A8840)
FABLE_UNUSED_OPEN_RETAIL_STUB(FableOpenRetailOperatorDelete_009A8840)

#undef FABLE_UNUSED_OPEN_RETAIL_STUB

int main()
{
    const bool passed =
        Check(false, 0, 0, 0, 0) &&
        Check(true, 0xDEADBEEF, 7, 1, 0) &&
        Check(true, 0x42474942, 8, 1, 1);
    if (!passed)
    {
        printf(
            "FABLETLC_OPEN_RETAIL_BANK FAIL allocations=%u open=%u "
            "stream_ctor=%u stream_dtor=%u owner_dtor=%u\n",
            g_allocations,
            g_diskOpenCalls,
            g_streamConstructCalls,
            g_streamDestroyCalls,
            g_ownerDestroyCalls);
        return 1;
    }

    printf("FABLETLC_OPEN_RETAIL_BANK PASS\n");
    return 0;
}
