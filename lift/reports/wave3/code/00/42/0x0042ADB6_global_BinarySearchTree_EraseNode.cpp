#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct BinarySearchTreeAllocatorOverlay {
    std::byte _pad0[0x4];
    std::byte field_4;
    std::byte _pad5[0x3];
    std::byte field_8;
    std::byte _pad9[0x3];
    std::byte field_C;
};

static_assert(offsetof(BinarySearchTreeAllocatorOverlay, field_4) == 0x4);
static_assert(offsetof(BinarySearchTreeAllocatorOverlay, field_8) == 0x8);
static_assert(offsetof(BinarySearchTreeAllocatorOverlay, field_C) == 0xC);

struct BinarySearchTreeOverlay {
    BinarySearchTreeAllocatorOverlay* allocator;
    std::int32_t count;
};

static_assert(offsetof(BinarySearchTreeOverlay, allocator) == 0x0);
static_assert(offsetof(BinarySearchTreeOverlay, count) == 0x4);

extern "C" void* __cdecl CMemoryAllocatorVariableSize__Erase(
    std::uint32_t param_2,
    void* param_3,
    void* param_4,
    void* param_5
);

void __thiscall BinarySearchTree_EraseNode(BinarySearchTreeOverlay* self, std::uint32_t param_2) {
    BinarySearchTreeAllocatorOverlay* const allocator = self->allocator;
    void* const memory = CMemoryAllocatorVariableSize__Erase(
        param_2,
        static_cast<void*>(&allocator->field_4),
        static_cast<void*>(&allocator->field_8),
        static_cast<void*>(&allocator->field_C)
    );
    if (memory != nullptr) {
        std::free(memory);
    }
    self->count = self->count + -1;
}