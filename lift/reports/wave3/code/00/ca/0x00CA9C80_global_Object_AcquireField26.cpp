#include <cstddef>
#include <cstdint>

struct Object_AcquireField26_Field10_Overlay {
    std::byte pad_00[0x14];
    std::int32_t field_14;
};
static_assert(offsetof(Object_AcquireField26_Field10_Overlay, field_14) == 0x14);

struct Object_AcquireField26_Overlay {
    void** vftable;                    // 0x00
    std::byte pad_04[0x24];           // 0x04..0x27
    Object_AcquireField26_Field10_Overlay* field_10; // 0x28
    std::byte pad_2C[0x6C];           // 0x2C..0x97
    void* field_26;                   // 0x98
    void* field_27;                   // 0x9C
};
static_assert(offsetof(Object_AcquireField26_Overlay, field_10) == 0x28);
static_assert(offsetof(Object_AcquireField26_Overlay, field_26) == 0x98);
static_assert(offsetof(Object_AcquireField26_Overlay, field_27) == 0x9C);

void __fastcall Object_AcquireField26(
    Object_AcquireField26_Overlay* self,
    void* /*unused_edx*/,
    void* /*unused_stack_arg*/
) {
    using Slot38Fn = std::int32_t(__thiscall*)(Object_AcquireField26_Overlay*, void*, void**);
    using Slot14Fn = void(__stdcall*)(void*);

    const auto result = reinterpret_cast<Slot38Fn>(self->vftable[0x38 / sizeof(void*)])(
        self,
        self->field_27,
        &self->field_26
    );

    if (result >= 0 && self->field_10->field_14 != 0) {
        void* const field26 = self->field_26;
        if (field26 != nullptr) {
            auto** const field26Vftable = *reinterpret_cast<void***>(field26);
            reinterpret_cast<Slot14Fn>(field26Vftable[0x14 / sizeof(void*)])(field26);
        }
    }
}