#include <cstddef>
#include <cstdlib>

struct CGuiControlTreeNode;

namespace std {
template <class T, class Allocator>
class list;
}

struct CGuiControlTreePane;

void __fastcall Catch_0047e8cc();
void __cdecl operator_delete(void* p);

struct CGuiControlTreePane_Overlay {
    std::byte pad_0x00[0x64];
    void* tree_block_0x64;
    void* tree_block_0x68;
};

static_assert(offsetof(CGuiControlTreePane_Overlay, tree_block_0x64) == 0x64);
static_assert(offsetof(CGuiControlTreePane_Overlay, tree_block_0x68) == 0x68);

void __fastcall CGuiControlTreePane_Destructor_4(void* param_1) {
    auto* const self = static_cast<CGuiControlTreePane_Overlay*>(param_1);
    std::list<CGuiControlTreeNode, std::allocator<CGuiControlTreeNode>>* unaff_EDI;

    if (self != nullptr) {
        CGuiControlTreePane::SortTreeRecursively(
            reinterpret_cast<CGuiControlTreePane*>(reinterpret_cast<std::byte*>(self) + 0x68),
            unaff_EDI);

        CGuiControlTreePane::SortTreeRecursively(
            reinterpret_cast<CGuiControlTreePane*>(reinterpret_cast<std::byte*>(self) + 0x64),
            unaff_EDI);

        void* const memory = self->tree_block_0x64;
        if (memory != nullptr) {
            std::free(memory);
        }

        Catch_0047e8cc();
        operator_delete(self);
    }
}