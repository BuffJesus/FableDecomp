#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CGuiControlTreePane_TreeStorage {
    void* memory;
};
static_assert(offsetof(CGuiControlTreePane_TreeStorage, memory) == 0x00);

struct CGuiControlTreePane_Overlay {
    std::byte pad_00[0x64];
    CGuiControlTreePane_TreeStorage treeStorage;
};
static_assert(offsetof(CGuiControlTreePane_Overlay, treeStorage) == 0x64);

// Observed call target at 0x00443C18: ecx = &this->treeStorage.
extern void __thiscall CGuiControlTreePane_SortTreeRecursively(CGuiControlTreePane_TreeStorage* self);

// Observed call target at 0x00443C2B: ecx = this.
extern void __thiscall Catch_0047E8CC(void* self);

void* __thiscall CGuiControlTreePane_Destructor_3(void* self, std::uint8_t flags)
{
    auto* const pane = static_cast<CGuiControlTreePane_Overlay*>(self);

    CGuiControlTreePane_SortTreeRecursively(&pane->treeStorage);

    void* const memory = pane->treeStorage.memory;
    if (memory != nullptr) {
        std::free(memory);
    }

    Catch_0047E8CC(self);

    if ((flags & 1u) != 0) {
        ::operator delete(self);
    }

    return self;
}