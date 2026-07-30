#include <bit>
#include <cstddef>
#include <cstdint>
#include <malloc.h>

struct CBankFile;
struct CEditWorld;
struct CIDrawWorldMap;
struct CWorldMap;

struct BatchInvokeStateOverlay {
    std::byte pad_0000[0x58];
    std::int32_t count;
};
static_assert(offsetof(BatchInvokeStateOverlay, count) == 0x58);

static inline CBankFile* GetBankFileAt1E0(BatchInvokeStateOverlay* state) {
    return reinterpret_cast<CBankFile*>(reinterpret_cast<std::byte*>(state) + 0x1E0);
}

using BatchInvokeCallback = int(__thiscall*)(void*, double);
using RawCodePtr = void(*)();

static inline long CallGetNextRegionOnRouteToRaw(
    CWorldMap* worldMapThisFromContext,
    std::uint32_t routeTargetFromEdi,
    long routeSourceFromEsi
) {
#if defined(_MSC_VER) && defined(_M_IX86)
    auto fn = reinterpret_cast<RawCodePtr>(CWorldMap::GetNextRegionOnRouteTo);
    long result;
    __asm {
        push esi
        push edi
        mov ecx, worldMapThisFromContext
        mov edi, routeTargetFromEdi
        mov esi, routeSourceFromEsi
        call fn
        mov result, eax
        pop edi
        pop esi
    }
    return result;
#else
    (void)worldMapThisFromContext;
    (void)routeTargetFromEdi;
    (void)routeSourceFromEsi;
    __assume(0);
#endif
}

static inline void CallVertexBufferFillDataRaw(
    BatchInvokeStateOverlay* stateFromEax,
    CEditWorld* editWorld,
    CBankFile* bankFile,
    void* stackArgBlockBase,
    std::int32_t firstShiftedWorldMap
) {
#if defined(_MSC_VER) && defined(_M_IX86)
    auto fn = reinterpret_cast<RawCodePtr>(VertexBuffer_FillData);
    __asm {
        push firstShiftedWorldMap
        push stackArgBlockBase
        push bankFile
        push editWorld
        push stateFromEax
        call fn
    }
#else
    (void)stateFromEax;
    (void)editWorld;
    (void)bankFile;
    (void)stackArgBlockBase;
    (void)firstShiftedWorldMap;
    __assume(0);
#endif
}

static inline void CallBufferSwapElementsRaw(CBankFile* bankFile, std::uint32_t* swapCookie) {
#if defined(_MSC_VER) && defined(_M_IX86)
    auto fn = reinterpret_cast<RawCodePtr>(Buffer_SwapElements);
    __asm {
        mov ecx, bankFile
        mov edx, swapCookie
        call fn
    }
#else
    (void)bankFile;
    (void)swapCookie;
    __assume(0);
#endif
}

static inline void CallAudioConvolveChannelsRaw(
    std::int32_t firstShiftedWorldMap,
    std::int32_t secondShiftedWorldMap,
    std::uint32_t firstEntrySize,
    std::uint32_t secondEntrySize,
    void* stackArgBlockBase,
    std::int32_t elementCount,
    std::uint32_t worldValue,
    std::uint32_t swapCookie
) {
#if defined(_MSC_VER) && defined(_M_IX86)
    auto fn = reinterpret_cast<RawCodePtr>(Audio_ConvolveChannels);
    __asm {
        push swapCookie
        push worldValue
        push elementCount
        push stackArgBlockBase
        push secondEntrySize
        mov edx, firstEntrySize
        mov eax, secondShiftedWorldMap
        mov ecx, firstShiftedWorldMap
        call fn
    }
#else
    (void)firstShiftedWorldMap;
    (void)secondShiftedWorldMap;
    (void)firstEntrySize;
    (void)secondEntrySize;
    (void)stackArgBlockBase;
    (void)elementCount;
    (void)worldValue;
    (void)swapCookie;
    __assume(0);
#endif
}

int __stdcall Batch_InvokeWithStackArgs_c8c730(
    BatchInvokeStateOverlay* stateFromEax,
    long routeSourceFromEsi,
    std::uint32_t routeTargetFromEdi,
    std::uint32_t param_1,
    std::uint32_t param_2,
    void* callbackRaw,
    CWorldMap* worldMapThisFromContext
) {
    if (stateFromEax->count <= 1) {
        return -0x83;
    }

    int result = CreatureHitNotification_WaitForCompletion();
    if (result != 0) {
        return result;
    }

    CEditWorld* editWorld = Indexed_GetElementOffsetSafe();
    const long nextRegion =
        CallGetNextRegionOnRouteToRaw(worldMapThisFromContext, routeTargetFromEdi, routeSourceFromEsi);

    const std::int32_t elementCount =
        *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(editWorld) + 0x04);

    CIDrawWorldMap* drawWorldMap = CEditWorld::DrawGetWorldMap(editWorld);
    const std::int32_t firstShiftedWorldMap =
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(drawWorldMap)) >>
        ((static_cast<unsigned char>(nextRegion) + 1U) & 0x1F);

    CBankFile* const bankFile = GetBankFileAt1E0(stateFromEax);
    const std::uint32_t firstEntrySize = CBankFile::GetEntryDataSize(bankFile, routeTargetFromEdi);

    const std::size_t pointerTableBytes =
        static_cast<std::size_t>(elementCount) * sizeof(std::byte*);
    const std::size_t perElementBytes =
        static_cast<std::size_t>(firstShiftedWorldMap) * sizeof(std::uint32_t);
    const std::size_t payloadBytes =
        static_cast<std::size_t>(elementCount) * perElementBytes;

    std::byte* const stackStorage =
        static_cast<std::byte*>(_alloca(pointerTableBytes + payloadBytes));
    auto* const stackArgBlockBase =
        reinterpret_cast<std::byte**>(stackStorage + payloadBytes);

    std::byte* current = reinterpret_cast<std::byte*>(stackArgBlockBase);
    const std::ptrdiff_t stackStep = static_cast<std::ptrdiff_t>(firstShiftedWorldMap) * -4;

    if (elementCount > 0) {
        for (std::int32_t i = 0; i < elementCount; ++i) {
            std::byte* const next = current + stackStep;
            stackArgBlockBase[i] = next;
            current = next;
        }
    }

    CallVertexBufferFillDataRaw(
        stateFromEax,
        editWorld,
        bankFile,
        stackArgBlockBase,
        firstShiftedWorldMap
    );

    const std::uint64_t packedParam =
        (static_cast<std::uint64_t>(param_2) << 32) | static_cast<std::uint64_t>(param_1);

    result = reinterpret_cast<BatchInvokeCallback>(callbackRaw)(
        stateFromEax,
        std::bit_cast<double>(packedParam)
    );
    if (result != 0) {
        return result;
    }

    result = VertexBuffer_WaitAndAllocate();
    if (result != 0) {
        return result;
    }

    editWorld = Indexed_GetElementOffsetSafe();
    const std::uint32_t worldValue =
        *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::byte*>(editWorld) + 0x04);

    drawWorldMap = CEditWorld::DrawGetWorldMap(editWorld);
    const std::int32_t secondShiftedWorldMap =
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(drawWorldMap)) >>
        ((static_cast<unsigned char>(nextRegion) + 1U) & 0x1F);

    const std::uint32_t secondEntrySize = CBankFile::GetEntryDataSize(bankFile, routeTargetFromEdi);

    std::uint32_t swapCookie = 0;
    CallBufferSwapElementsRaw(bankFile, &swapCookie);

    CallAudioConvolveChannelsRaw(
        firstShiftedWorldMap,
        secondShiftedWorldMap,
        firstEntrySize,
        secondEntrySize,
        stackArgBlockBase,
        elementCount,
        worldValue,
        swapCookie
    );

    return 0;
}