#include <cstddef>
#include <cstdint>
#include <new>

struct VectorInsertElement_ControlOverlay {
    std::int32_t ref_count;                  // 0x00
    void (__thiscall* release)(void*);       // 0x04
    void* release_this;                      // 0x08
};
static_assert(offsetof(VectorInsertElement_ControlOverlay, ref_count) == 0x00);
static_assert(offsetof(VectorInsertElement_ControlOverlay, release) == 0x04);
static_assert(offsetof(VectorInsertElement_ControlOverlay, release_this) == 0x08);
static_assert(sizeof(VectorInsertElement_ControlOverlay) == 0x0C);

struct VectorInsertElement_SlotOverlay {
    std::uint32_t value0;                                   // 0x00
    std::uint32_t value1;                                   // 0x04
    VectorInsertElement_ControlOverlay* control;            // 0x08
};
static_assert(offsetof(VectorInsertElement_SlotOverlay, value0) == 0x00);
static_assert(offsetof(VectorInsertElement_SlotOverlay, value1) == 0x04);
static_assert(offsetof(VectorInsertElement_SlotOverlay, control) == 0x08);
static_assert(sizeof(VectorInsertElement_SlotOverlay) == 0x0C);

struct VectorInsertElement_Param3Overlay {
    std::uint32_t value0;                                   // 0x00
    VectorInsertElement_ControlOverlay* control;            // 0x04
};
static_assert(offsetof(VectorInsertElement_Param3Overlay, value0) == 0x00);
static_assert(offsetof(VectorInsertElement_Param3Overlay, control) == 0x04);
static_assert(sizeof(VectorInsertElement_Param3Overlay) == 0x08);

struct VectorInsertElement_ThisOverlay {
    std::byte pad_00[0x04];
    VectorInsertElement_SlotOverlay* next;                  // 0x04
    VectorInsertElement_SlotOverlay* end;                   // 0x08
    std::byte pad_0C[0x01];
    std::uint8_t modified;                                  // 0x0D
};
static_assert(offsetof(VectorInsertElement_ThisOverlay, next) == 0x04);
static_assert(offsetof(VectorInsertElement_ThisOverlay, end) == 0x08);
static_assert(offsetof(VectorInsertElement_ThisOverlay, modified) == 0x0D);

extern void __thiscall Vector_Reallocate(
    VectorInsertElement_ThisOverlay* self,
    VectorInsertElement_SlotOverlay* insertion_ptr,
    const VectorInsertElement_SlotOverlay* local_element,
    const std::uint32_t* const* source_slot,
    int a4,
    int a5);

void __thiscall Vector_InsertElement(
    VectorInsertElement_ThisOverlay* self,
    const std::uint32_t* param_2,
    const VectorInsertElement_Param3Overlay* param_3)
{
    const std::uint32_t local_c = *param_2;
    VectorInsertElement_ControlOverlay* local_4 = param_3->control;
    const std::uint32_t local_8 = param_3->value0;

    if (local_4 != nullptr) {
        local_4->ref_count = local_4->ref_count + 1;
    }

    VectorInsertElement_SlotOverlay* puVar1 = self->next;
    if (puVar1 == self->end) {
        const VectorInsertElement_SlotOverlay local_element{local_c, local_8, local_4};
        Vector_Reallocate(self, puVar1, &local_element, &param_2, 1, 1);
    } else {
        if (puVar1 != nullptr) {
            puVar1->value0 = local_c;
            puVar1->value1 = local_8;
            puVar1->control = local_4;
            if (local_4 != nullptr) {
                local_4->ref_count = local_4->ref_count + 1;
            }
        }

        self->next = reinterpret_cast<VectorInsertElement_SlotOverlay*>(
            reinterpret_cast<std::byte*>(self->next) + 0x0C);
    }

    VectorInsertElement_ControlOverlay* piVar2 = local_4;
    if ((local_4 != nullptr) && (local_4->ref_count = local_4->ref_count + -1, local_4->ref_count == 0)) {
        local_4->release(local_4->release_this);
        operator delete(piVar2);
    }

    self->modified = 1;
}