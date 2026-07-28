#include <cstddef>
#include <cstdint>

struct LinkedListNodeOverlay {
    LinkedListNodeOverlay* next; // 0x00
    LinkedListNodeOverlay* prev; // 0x04
};

static_assert(offsetof(LinkedListNodeOverlay, next) == 0x00);
static_assert(offsetof(LinkedListNodeOverlay, prev) == 0x04);

extern LinkedListNodeOverlay* LinkedList_AllocateNode(std::uint32_t value);

LinkedListNodeOverlay** LinkedList_Insert_Before(
    LinkedListNodeOverlay** outInsertedNode,
    LinkedListNodeOverlay* node,
    std::uint32_t value)
{
    LinkedListNodeOverlay* const insertedNode = LinkedList_AllocateNode(value);
    LinkedListNodeOverlay* const previousNode = node->prev;

    insertedNode->next = node;
    insertedNode->prev = previousNode;
    previousNode->next = insertedNode;
    node->prev = insertedNode;
    *outInsertedNode = insertedNode;
    return outInsertedNode;
}