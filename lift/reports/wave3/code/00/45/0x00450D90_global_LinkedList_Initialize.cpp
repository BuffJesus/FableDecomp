#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct LinkedListNodeOverlay {
    LinkedListNodeOverlay* next;   // 0x00
    LinkedListNodeOverlay* prev;   // 0x04
    std::byte reserved[0x18];      // 0x08
};

static_assert(offsetof(LinkedListNodeOverlay, next) == 0x00);
static_assert(offsetof(LinkedListNodeOverlay, prev) == 0x04);
static_assert(sizeof(LinkedListNodeOverlay) == 0x20);

void** __fastcall LinkedList_Initialize(void** listHead)
{
    *listHead = nullptr;

    auto* node = static_cast<LinkedListNodeOverlay*>(std::malloc(0x20));
    node->next = node;
    node->prev = node;

    *listHead = node;
    return listHead;
}