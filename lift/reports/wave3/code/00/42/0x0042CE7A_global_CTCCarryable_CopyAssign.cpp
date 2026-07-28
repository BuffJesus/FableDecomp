#include <cstddef>
#include <cstdint>

class CTCCarryable {
public:
    void __thiscall OnKill();
};

struct CMemoryAllocatorVariableSize {
    static std::uint32_t __cdecl GetNoAllocatedAreas(void* allocator);
};

struct CTCCarryable_SurfaceOverlay {
    std::byte _pad0[0x04];
    void* field_0x04;
    std::uint32_t field_0x08;
    std::uint32_t field_0x0C;
};

static_assert(offsetof(CTCCarryable_SurfaceOverlay, field_0x04) == 0x04);
static_assert(offsetof(CTCCarryable_SurfaceOverlay, field_0x08) == 0x08);
static_assert(offsetof(CTCCarryable_SurfaceOverlay, field_0x0C) == 0x0C);

struct CTCCarryable_Overlay {
    CTCCarryable_SurfaceOverlay* field_0x00;
    std::uint32_t field_0x04;
};

static_assert(offsetof(CTCCarryable_Overlay, field_0x00) == 0x00);
static_assert(offsetof(CTCCarryable_Overlay, field_0x04) == 0x04);

// Observed call shape:
//   ECX = self
//   push self_overlay->field_0x00
//   push other_overlay->field_0x00->field_0x04
//   call CSurface_Clone_Recursive
void* __thiscall CSurface_Clone_Recursive(
    CTCCarryable* self,
    void* sourceSurface,
    void* destinationBase);

CTCCarryable* __thiscall CTCCarryable_CopyAssign(CTCCarryable* self, CTCCarryable* other) {
    if (self != other) {
        self->CTCCarryable::OnKill();

        auto* const selfOverlay = reinterpret_cast<CTCCarryable_Overlay*>(self);
        auto* const otherOverlay = reinterpret_cast<CTCCarryable_Overlay*>(other);

        selfOverlay->field_0x04 = 0;

        if (otherOverlay->field_0x00->field_0x04 == nullptr) {
            selfOverlay->field_0x00->field_0x04 = nullptr;
            selfOverlay->field_0x00->field_0x08 =
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(selfOverlay->field_0x00));
            selfOverlay->field_0x00->field_0x0C =
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(selfOverlay->field_0x00));
        } else {
            reinterpret_cast<CTCCarryable_Overlay*>(self)->field_0x00->field_0x04 =
                CSurface_Clone_Recursive(
                    self,
                    otherOverlay->field_0x00->field_0x04,
                    reinterpret_cast<CTCCarryable_Overlay*>(self)->field_0x00);

            reinterpret_cast<CTCCarryable_Overlay*>(self)->field_0x00->field_0x08 =
                CMemoryAllocatorVariableSize::GetNoAllocatedAreas(
                    reinterpret_cast<CTCCarryable_Overlay*>(self)->field_0x00->field_0x04);

            reinterpret_cast<CTCCarryable_Overlay*>(self)->field_0x00->field_0x0C =
                CMemoryAllocatorVariableSize::GetNoAllocatedAreas(
                    reinterpret_cast<CTCCarryable_Overlay*>(self)->field_0x00->field_0x04);

            selfOverlay->field_0x04 = otherOverlay->field_0x04;
        }
    }

    return self;
}