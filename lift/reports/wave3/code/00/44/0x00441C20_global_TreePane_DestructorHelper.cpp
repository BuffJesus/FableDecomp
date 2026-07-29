#include <cstddef>
#include <cstdint>
#include <cstdlib>

namespace
{
    struct TreePaneStorageOverlay
    {
        void* allocated_nodes;
    };
    static_assert(offsetof(TreePaneStorageOverlay, allocated_nodes) == 0x00);

    struct TreePaneDestructorHelperOverlay
    {
        std::byte pad_00[0x1C];
        void* callback_array_begin;   // 0x1C
        void* callback_array_end;     // 0x20
        std::byte pad_24[0x18];
        TreePaneStorageOverlay tree_storage; // 0x3C
    };

    static_assert(offsetof(TreePaneDestructorHelperOverlay, callback_array_begin) == 0x1C);
    static_assert(offsetof(TreePaneDestructorHelperOverlay, callback_array_end) == 0x20);
    static_assert(offsetof(TreePaneDestructorHelperOverlay, tree_storage) == 0x3C);

    using SortTreeRecursivelyFn = void(__thiscall*)(void*);
    using IndirectEntryCallFn = void(__thiscall*)(void*, int);
}

void __fastcall TreePane_DestructorHelper(TreePaneDestructorHelperOverlay* self)
{
    reinterpret_cast<SortTreeRecursivelyFn>(0x0043F230)(&self->tree_storage);

    void* memory = self->tree_storage.allocated_nodes;
    if (memory != nullptr)
    {
        std::free(memory);
    }

    auto* current = static_cast<std::byte*>(self->callback_array_begin);
    auto* end = static_cast<std::byte*>(self->callback_array_end);
    while (current != end)
    {
        auto** table = *reinterpret_cast<void***>(current);
        auto call = reinterpret_cast<IndirectEntryCallFn>(table[0]);
        call(current, 0);
        current += 8;
    }

    if (self->callback_array_begin != nullptr)
    {
        std::free(self->callback_array_begin);
    }
}