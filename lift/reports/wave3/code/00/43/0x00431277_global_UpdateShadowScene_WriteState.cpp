#include <cstddef>
#include <cstdint>

struct ShadowSceneAllocNodeOverlay {
    std::byte pad00[0x08];
    ShadowSceneAllocNodeOverlay* next;   // 0x08
    std::byte pad0C[0x04];
    std::uint32_t value10;               // 0x10
    std::uint32_t value14;               // 0x14
};

static_assert(offsetof(ShadowSceneAllocNodeOverlay, next) == 0x08);
static_assert(offsetof(ShadowSceneAllocNodeOverlay, value10) == 0x10);
static_assert(offsetof(ShadowSceneAllocNodeOverlay, value14) == 0x14);

struct UpdateShadowSceneWriteStateOverlay {
    ShadowSceneAllocNodeOverlay* sentinel; // 0x00
    std::uint32_t rootValue;               // 0x04
};

static_assert(offsetof(UpdateShadowSceneWriteStateOverlay, sentinel) == 0x00);
static_assert(offsetof(UpdateShadowSceneWriteStateOverlay, rootValue) == 0x04);

// Ghidra labeled the traversal helper as CMemoryAllocatorVariableSize::GetNoAllocatedAreas,
// but the observed call shape is a direct single-argument call, so preserve that ABI shape.
extern ShadowSceneAllocNodeOverlay* __cdecl CMemoryAllocatorVariableSize_GetNoAllocatedAreas(
    ShadowSceneAllocNodeOverlay* node);

void __fastcall UpdateShadowScene_WriteState(
    CDataOutputStream* stream,
    UpdateShadowSceneWriteStateOverlay* state)
{
    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(state->rootValue));

    ShadowSceneAllocNodeOverlay* node = state->sentinel->next;
    if (node != state->sentinel) {
        do {
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(node->value10));
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(node->value14));
            node = CMemoryAllocatorVariableSize_GetNoAllocatedAreas(node);
        } while (node != state->sentinel);
    }
}