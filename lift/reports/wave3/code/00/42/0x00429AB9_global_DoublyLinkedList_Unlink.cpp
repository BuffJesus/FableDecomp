#include <cstddef>
#include <cstdlib>

struct DoublyLinkedListNodeOverlay {
    DoublyLinkedListNodeOverlay* next; // 0x00
    DoublyLinkedListNodeOverlay* prev; // 0x04
};

static_assert(offsetof(DoublyLinkedListNodeOverlay, next) == 0x00);
static_assert(offsetof(DoublyLinkedListNodeOverlay, prev) == 0x04);

void DoublyLinkedList_Unlink(DoublyLinkedListNodeOverlay** listHead, DoublyLinkedListNodeOverlay* node)
{
    DoublyLinkedListNodeOverlay* prev = node->prev;
    DoublyLinkedListNodeOverlay* next = node->next;

    prev->next = next;
    next->prev = prev;
    std::free(node);
    *listHead = next;
}