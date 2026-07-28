#include <algorithm>
#include <cstddef>
#include <cstdint>

struct CEnginePrimitiveHandleOverlay {
    void** vfptr;
    std::uint32_t unknown_04;
};
static_assert(sizeof(CEnginePrimitiveHandleOverlay) == 0x8);

struct VectorOverlay {
    std::uint32_t unknown_00;
    CEnginePrimitiveHandleOverlay* finish;
};
static_assert(offsetof(VectorOverlay, finish) == 0x4);

CEnginePrimitiveHandleOverlay* __thiscall Vector_EraseRange(
    VectorOverlay* self,
    CEnginePrimitiveHandleOverlay* first,
    CEnginePrimitiveHandleOverlay* last)
{
    CEnginePrimitiveHandleOverlay* const newFinish = std::move(last, self->finish, first);
    CEnginePrimitiveHandleOverlay* const oldFinish = self->finish;

    for (CEnginePrimitiveHandleOverlay* it = newFinish; it != oldFinish; ++it) {
        using RawDtor = void(__thiscall*)(CEnginePrimitiveHandleOverlay*, int);
        const auto rawDtor = reinterpret_cast<RawDtor>(it->vfptr[0]);
        rawDtor(it, 0);
    }

    self->finish = newFinish;
    return first;
}