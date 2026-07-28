#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct DoublyLinkedListNodeOverlay
{
    DoublyLinkedListNodeOverlay* next; // 0x00
    DoublyLinkedListNodeOverlay* prev; // 0x04
    std::uint32_t reserved;            // 0x08
};

static_assert(offsetof(DoublyLinkedListNodeOverlay, next) == 0x0);
static_assert(offsetof(DoublyLinkedListNodeOverlay, prev) == 0x4);
static_assert(offsetof(DoublyLinkedListNodeOverlay, reserved) == 0x8);
static_assert(sizeof(DoublyLinkedListNodeOverlay) == 0xC);

void** __fastcall DoublyLinkedList_InitializeEmpty(void** listHeadStorage)
{
    *listHeadStorage = nullptr;

    void* const sentinelAllocation = std::malloc(0xC);
    auto* const sentinel = static_cast<DoublyLinkedListNodeOverlay*>(sentinelAllocation);

    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    *listHeadStorage = sentinelAllocation;

    return listHeadStorage;
}