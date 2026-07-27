#include <cstddef>
#include <cstdint>
#include <malloc.h>

struct CBankFile;
struct CEditWorld;
struct CIDrawWorldMap;
struct CWorldMap;

extern "C" int CreatureHitNotification_WaitForCompletion();
extern "C" CEditWorld* Indexed_GetElementOffsetSafe();
extern "C" long CWorldMap_GetNextRegionOnRouteTo(CWorldMap* self, unsigned long regionId, long routeState);
extern "C" CIDrawWorldMap* CEditWorld_DrawGetWorldMap(CEditWorld* self);
extern "C" unsigned long CBankFile_GetEntryDataSize(CBankFile* self, unsigned long entryId);
extern "C" void VertexBuffer_FillData(...);
extern "C" int VertexBuffer_WaitAndAllocate();
extern "C" void __fastcall Buffer_SwapElements(void* tableBase, std::uint32_t* outValue);
extern "C" void Audio_ConvolveChannels(...);

struct CEditWorldOverlay {
    std::byte pad_00[0x04];
    std::uint32_t regionCount;
};
static_assert(offsetof(CEditWorldOverlay, regionCount) == 0x04);

struct BatchInvokeContextOverlay {
    std::byte pad_0000[0x58];
    int field_58;
    std::byte pad_005c[0x1E0 - 0x5C];
    CBankFile bankFile_1e0;
};
static_assert(offsetof(BatchInvokeContextOverlay, field_58) == 0x58);
static_assert(offsetof(BatchInvokeContextOverlay, bankFile_1e0) == 0x1E0);

using BatchInvokeCallback = int(__thiscall*)(BatchInvokeContextOverlay*, std::uint64_t);

static inline std::size_t Align4(std::size_t value) {
    return (value + 3u) & ~std::size_t(3);
}

// Ambient recovered inputs supplied by the lifting context for this function body:
//   BatchInvokeContextOverlay* in_EAX
//   CWorldMap* this_00
//   unsigned long unaff_EDI
//   long unaff_ESI
int __stdcall Batch_InvokeWithStackArgs_c8c730(
    std::uint32_t param_1,
    std::uint32_t param_2,
    BatchInvokeCallback param_3)
{
    static_assert(sizeof(void*) == 4, "This reversal models the original 32-bit x86 stack layout.");

    if (in_EAX->field_58 <= 1) {
        return -0x83;
    }

    int result = CreatureHitNotification_WaitForCompletion();
    if (result != 0) {
        return result;
    }

    CEditWorld* const editWorld = Indexed_GetElementOffsetSafe();
    const long nextRegion = CWorldMap_GetNextRegionOnRouteTo(this_00, unaff_EDI, unaff_ESI);
    const int regionCount = static_cast<int>(
        reinterpret_cast<CEditWorldOverlay*>(editWorld)->regionCount);

    CIDrawWorldMap* const drawWorldMap = CEditWorld_DrawGetWorldMap(editWorld);
    const int shiftedMapValue =
        static_cast<int>(reinterpret_cast<std::uintptr_t>(drawWorldMap)) >>
        ((static_cast<unsigned char>(nextRegion) + 1U) & 0x1F);

    CBankFile* const bankFile = &in_EAX->bankFile_1e0;
    CBankFile_GetEntryDataSize(bankFile, unaff_EDI);

    const int iVar3 = regionCount * -4;
    std::byte* const tableBase =
        static_cast<std::byte*>(_alloca(Align4(static_cast<std::size_t>(regionCount) * 4u)));
    std::byte* puVar10 = tableBase;
    std::byte* puVar11 = tableBase;
    int local_10 = 0;

    if (regionCount > 0) {
        do {
            const int iVar4 = shiftedMapValue * -4;
            puVar11 = static_cast<std::byte*>(_alloca(Align4(static_cast<std::size_t>(shiftedMapValue) * 4u)));
            *reinterpret_cast<std::byte**>(tableBase + iVar3 + local_10 * 4) = puVar11;
            local_10 = local_10 + 1;
            puVar10 = puVar11;
            (void)puVar10;
            (void)iVar4;
        } while (local_10 < regionCount);
    }

    *reinterpret_cast<int*>(puVar11 - 0x04) = shiftedMapValue;
    *reinterpret_cast<std::byte**>(puVar11 - 0x08) = tableBase + iVar3;
    *reinterpret_cast<CBankFile**>(puVar11 - 0x0C) = bankFile;
    *reinterpret_cast<CEditWorld**>(puVar11 - 0x10) = editWorld;
    *reinterpret_cast<BatchInvokeContextOverlay**>(puVar11 - 0x14) = in_EAX;

    VertexBuffer_FillData(
        *reinterpret_cast<BatchInvokeContextOverlay**>(puVar11 - 0x14),
        *reinterpret_cast<CEditWorld**>(puVar11 - 0x10),
        *reinterpret_cast<CBankFile**>(puVar11 - 0x0C),
        *reinterpret_cast<void**>(puVar11 - 0x08),
        *reinterpret_cast<int*>(puVar11 - 0x04));

    *reinterpret_cast<std::uint64_t*>(puVar11 - 0x08) =
        (static_cast<std::uint64_t>(param_2) << 32) | static_cast<std::uint64_t>(param_1);

    auto* const puVar12 = reinterpret_cast<unsigned long*>(puVar11 - 0x0C);

    result = param_3(in_EAX, (static_cast<std::uint64_t>(param_2) << 32) | static_cast<std::uint64_t>(param_1));
    if (result != 0) {
        return result;
    }

    result = VertexBuffer_WaitAndAllocate();
    if (result != 0) {
        return result;
    }

    CEditWorld* const editWorld2 = Indexed_GetElementOffsetSafe();
    const std::uint32_t regionCount2 =
        reinterpret_cast<CEditWorldOverlay*>(editWorld2)->regionCount;

    CEditWorld_DrawGetWorldMap(editWorld2);

    const unsigned long entryDataSize = CBankFile_GetEntryDataSize(bankFile, *puVar12);

    std::uint32_t local_1c = 0;
    Buffer_SwapElements(tableBase + iVar3, &local_1c);

    Audio_ConvolveChannels(local_1c, tableBase + iVar3, regionCount, regionCount2, entryDataSize);
    return 0;
}