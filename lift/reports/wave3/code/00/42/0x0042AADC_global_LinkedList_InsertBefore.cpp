struct LinkedListNode12Overlay
{
    LinkedListNode12Overlay* next; // 0x00
    LinkedListNode12Overlay* prev; // 0x04
    std::uint32_t payload;         // 0x08
};

static_assert(offsetof(LinkedListNode12Overlay, next) == 0x00);
static_assert(offsetof(LinkedListNode12Overlay, prev) == 0x04);
static_assert(offsetof(LinkedListNode12Overlay, payload) == 0x08);
static_assert(sizeof(LinkedListNode12Overlay) == 0x0C);

extern "C" LinkedListNode12Overlay* LinkedList_AllocateNode_12bytes(std::uint32_t value);

extern "C" LinkedListNode12Overlay** LinkedList_InsertBefore(
    LinkedListNode12Overlay** outInsertedNode,
    LinkedListNode12Overlay* beforeNode,
    std::uint32_t value)
{
    LinkedListNode12Overlay* const insertedNode = LinkedList_AllocateNode_12bytes(value);
    LinkedListNode12Overlay* const previousNode = beforeNode->prev;

    insertedNode->next = beforeNode;
    insertedNode->prev = previousNode;
    previousNode->next = insertedNode;
    beforeNode->prev = insertedNode;
    *outInsertedNode = insertedNode;

    return outInsertedNode;
}