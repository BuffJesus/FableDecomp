#include <bit>
#include <cstddef>
#include <cstdint>
#include <malloc.h>

struct CBankFile {
    std::byte opaque;
};

struct CEditWorld;
struct CIDrawWorldMap;
struct CWorldMap;

struct CEditWorldOverlay {
    std::byte pad_0000[0x4];
    std::int32_t entryCount;
};
static_assert(offsetof(CEditWorldOverlay, entryCount) == 0x4);

struct Batch_InvokeWithStackArgs_c8c730_Overlay {
    std::byte pad_0000[0x58];
    std::int32_t activeCount;
    std::byte pad_005C[0x184];
    CBankFile bankFile;
};
static_assert(offsetof(Batch_InvokeWithStackArgs_c8c730_Overlay, activeCount) == 0x58);
static_assert(offsetof(Batch_InvokeWithStackArgs_c8c730_Overlay, bankFile) == 0x1E0);

struct Batch_InvokeWithStackArgs_c8c730_IncomingRegs {
    Batch_InvokeWithStackArgs_c8c730_Overlay* eax_self;
    CWorldMap* ecx_worldMap;
    std::intptr_t esi_routeState;
    std::uint32_t edi_region;
};

struct CWorldMap_GetNextRegionOnRouteTo_RawCall {
    CWorldMap* ecx_this;
    std::uint32_t edi_region;
    std::intptr_t esi_routeState;
};

struct Audio_ConvolveChannels_RawCall {
    int ecx_originalStride;
    std::uint32_t edx_initialEntryDataSize;
    int eax_updatedStride;
    std::uint32_t stack_swappedValue;
    void** stack_blocks;
    int stack_originalEntryCount;
    int stack_refreshedEntryCount;
    std::uint32_t stack_currentEntryDataSize;
};

using BatchCallback = int(__thiscall*)(Batch_InvokeWithStackArgs_c8c730_Overlay*, double);

extern "C" int __cdecl CreatureHitNotification_WaitForCompletion();
extern "C" CEditWorld* __fastcall Indexed_GetElementOffsetSafe(
    Batch_InvokeWithStackArgs_c8c730_Overlay* self,
    std::int32_t index
);
extern "C" long CWorldMap_GetNextRegionOnRouteTo_raw(
    const CWorldMap_GetNextRegionOnRouteTo_RawCall& call
);
extern "C" CIDrawWorldMap* __thiscall CEditWorld_DrawGetWorldMap(CEditWorld* self);
extern "C" std::uint32_t __fastcall CBankFile_GetEntryDataSize(
    CBankFile* self,
    std::uint32_t entryIndex
);
extern "C" void __cdecl VertexBuffer_FillData(
    Batch_InvokeWithStackArgs_c8c730_Overlay* self,
    CEditWorld* editWorld,
    CBankFile* bankFile,
    void** stackBlocks,
    int stride
);
extern "C" int __cdecl VertexBuffer_WaitAndAllocate();
extern "C" void __fastcall Buffer_SwapElements(
    CBankFile* self,
    std::uint32_t* outValue
);
extern "C" void Audio_ConvolveChannels_raw(
    const Audio_ConvolveChannels_RawCall& call
);
extern "C" const Batch_InvokeWithStackArgs_c8c730_IncomingRegs&
Batch_InvokeWithStackArgs_c8c730_GetIncomingRegs();

int __cdecl Batch_InvokeWithStackArgs_c8c730(
    std::uint32_t param_1,
    std::uint32_t param_2,
    BatchCallback callback
) {
    const Batch_InvokeWithStackArgs_c8c730_IncomingRegs& regs =
        Batch_InvokeWithStackArgs_c8c730_GetIncomingRegs();
    auto* const self = regs.eax_self;

    if (self->activeCount <= 1) {
        return -0x83;
    }

    int result = CreatureHitNotification_WaitForCompletion();
    if (result != 0) {
        return result;
    }

    CEditWorld* const editWorld = Indexed_GetElementOffsetSafe(self, -1);
    const long nextRegion = CWorldMap_GetNextRegionOnRouteTo_raw({
        regs.ecx_worldMap,
        regs.edi_region,
        regs.esi_routeState,
    });

    const int originalEntryCount =
        reinterpret_cast<CEditWorldOverlay*>(editWorld)->entryCount;

    const int originalStride =
        static_cast<int>(reinterpret_cast<std::uintptr_t>(CEditWorld_DrawGetWorldMap(editWorld))) >>
        ((static_cast<unsigned char>(nextRegion) + 1U) & 0x1F);

    CBankFile* const bankFile = &self->bankFile;
    const std::uint32_t initialEntryDataSize = CBankFile_GetEntryDataSize(bankFile, 0);

    void** const stackBlocks = static_cast<void**>(
        _alloca((static_cast<std::uint32_t>(originalEntryCount) * 4U + 3U) & ~3U)
    );

    void* currentBlock = stackBlocks;
    for (int i = 0; i < originalEntryCount; ++i) {
        currentBlock = _alloca((static_cast<std::uint32_t>(originalStride) * 4U + 3U) & ~3U);
        stackBlocks[i] = currentBlock;
    }

    VertexBuffer_FillData(self, editWorld, bankFile, stackBlocks, originalStride);

    const double callbackArg =
        std::bit_cast<double>((static_cast<std::uint64_t>(param_2) << 32) | param_1);

    result = callback(self, callbackArg);
    if (result != 0) {
        return result;
    }

    result = VertexBuffer_WaitAndAllocate();
    if (result != 0) {
        return result;
    }

    CEditWorld* const refreshedEditWorld = Indexed_GetElementOffsetSafe(self, -1);
    const int refreshedEntryCount =
        reinterpret_cast<CEditWorldOverlay*>(refreshedEditWorld)->entryCount;

    const int updatedStride =
        static_cast<int>(reinterpret_cast<std::uintptr_t>(CEditWorld_DrawGetWorldMap(refreshedEditWorld))) >>
        ((static_cast<unsigned char>(nextRegion) + 1U) & 0x1F);

    const std::uint32_t currentEntryDataSize = CBankFile_GetEntryDataSize(bankFile, 0);

    std::uint32_t swappedValue = 0;
    Buffer_SwapElements(bankFile, &swappedValue);

    Audio_ConvolveChannels_raw({
        originalStride,
        initialEntryDataSize,
        updatedStride,
        swappedValue,
        stackBlocks,
        originalEntryCount,
        refreshedEntryCount,
        currentEntryDataSize,
    });

    return 0;
}