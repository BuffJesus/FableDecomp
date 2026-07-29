#include <cstddef>
#include <cstdlib>
#include <list>

struct CGuiControlTreeNode;

struct CGuiControlTreePane_Overlay {
    std::byte pad_00[0x1C];
    void* itemsBegin; // 0x1C
    void* itemsEnd;   // 0x20
    std::byte pad_24[0x04];
};

static_assert(offsetof(CGuiControlTreePane_Overlay, itemsBegin) == 0x1C);
static_assert(offsetof(CGuiControlTreePane_Overlay, itemsEnd) == 0x20);

void __fastcall CGuiControlTreePane_Destructor_1(
    void* self,
    std::list<CGuiControlTreeNode, std::allocator<CGuiControlTreeNode>>* sortListFromEdi)
{
    if (self != nullptr) {
        CGuiControlTreePane::SortTreeRecursively(
            reinterpret_cast<CGuiControlTreePane*>(reinterpret_cast<std::byte*>(self) + 0x28),
            sortListFromEdi);

        auto* const pane = static_cast<CGuiControlTreePane_Overlay*>(self);
        auto* current = static_cast<std::byte*>(pane->itemsBegin);
        auto* const end = static_cast<std::byte*>(pane->itemsEnd);

        while (current != end) {
            using RawEntry0Call = void(__thiscall*)(void*, int);
            auto* const vftable = *reinterpret_cast<void***>(current);
            reinterpret_cast<RawEntry0Call>(vftable[0])(current, 0);
            current += 8;
        }

        if (pane->itemsBegin != nullptr) {
            std::free(pane->itemsBegin);
        }

        ::operator delete(self);
    }
}