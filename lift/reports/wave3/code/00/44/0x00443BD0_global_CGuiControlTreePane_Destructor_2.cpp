#include <cstddef>
#include <cstdlib>

struct CGuiControlTreePane;

struct CGuiControlTreePane_Overlay {
    std::byte pad_00[0x64];
    void* tree_storage;
};
static_assert(offsetof(CGuiControlTreePane_Overlay, tree_storage) == 0x64);

// Observed as `ecx = this + 0x64; call ...`
extern void __thiscall SortTreeRecursively(void* tree_storage_at_0x64);

// Observed as `ecx = this; call ...`
extern void __thiscall Catch_0047E8CC(CGuiControlTreePane* self);

void* __thiscall CGuiControlTreePane_Destructor_2(CGuiControlTreePane* self, unsigned char flags)
{
    auto* const overlay = reinterpret_cast<CGuiControlTreePane_Overlay*>(self);

    SortTreeRecursively(&overlay->tree_storage);

    void* const memory = overlay->tree_storage;
    if (memory != nullptr) {
        std::free(memory);
    }

    Catch_0047E8CC(self);

    if ((flags & 1) != 0) {
        ::operator delete(self);
    }

    return self;
}