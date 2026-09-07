struct CMemoryAllocatorVariableSize;

struct ShadowAllocatorEntryOverlay {
    std::uint8_t _pad0[0x10];
    std::uint32_t type;
    bool enabled;
};

static_assert(offsetof(ShadowAllocatorEntryOverlay, type) == 0x10);
static_assert(offsetof(ShadowAllocatorEntryOverlay, enabled) == 0x14);

struct ShadowAllocatorSentinelOverlay {
    std::uint8_t _pad0[0x8];
    CMemoryAllocatorVariableSize* firstArea;
};

static_assert(offsetof(ShadowAllocatorSentinelOverlay, firstArea) == 0x8);

struct ShadowAllocatorDataOverlay {
    ShadowAllocatorSentinelOverlay* sentinel;
    std::uint32_t countValue;
};

static_assert(offsetof(ShadowAllocatorDataOverlay, sentinel) == 0x0);
static_assert(offsetof(ShadowAllocatorDataOverlay, countValue) == 0x4);

extern CMemoryAllocatorVariableSize* __cdecl CMemoryAllocatorVariableSize_GetNoAllocatedAreas(
    CMemoryAllocatorVariableSize* allocator);

void __fastcall Shadow_WriteAllocatorData(
    CDataOutputStream* stream,
    ShadowAllocatorDataOverlay* allocatorData)
{
    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(allocatorData->countValue));

    ShadowAllocatorSentinelOverlay* const sentinel = allocatorData->sentinel;
    CMemoryAllocatorVariableSize* area = sentinel->firstArea;
    if (area != reinterpret_cast<CMemoryAllocatorVariableSize*>(sentinel)) {
        do {
            const auto* entry = reinterpret_cast<const ShadowAllocatorEntryOverlay*>(area);
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(entry->type));
            CDataOutputStream::WriteEBOOL(stream, entry->enabled);
            area = CMemoryAllocatorVariableSize_GetNoAllocatedAreas(area);
        } while (area != reinterpret_cast<CMemoryAllocatorVariableSize*>(sentinel));
    }
}