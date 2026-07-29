#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CGuiControlTreePane_Destructor_5_Overlay {
    std::byte pad_00[0x64];
    void* tree_storage_at_64;
    std::byte tree_state_at_68[];
};

static_assert(offsetof(CGuiControlTreePane_Destructor_5_Overlay, tree_storage_at_64) == 0x64);

extern void __thiscall CGuiControlTreePane::SortTreeRecursively(void* self);
extern void __thiscall Catch_0047E8CC(void* self);

void __fastcall CGuiControlTreePane_Destructor_5(void* param_1) {
    if (param_1 != nullptr) {
        auto* const self = static_cast<CGuiControlTreePane_Destructor_5_Overlay*>(param_1);

        CGuiControlTreePane::SortTreeRecursively(reinterpret_cast<std::byte*>(self) + 0x68);
        CGuiControlTreePane::SortTreeRecursively(reinterpret_cast<std::byte*>(self) + 0x64);

        void* const memory = self->tree_storage_at_64;
        if (memory != nullptr) {
            std::free(memory);
        }

        Catch_0047E8CC(self);
        ::operator delete(param_1);
    }
}