struct LinkedListNodeOverlay
{
    LinkedListNodeOverlay* prev; // 0x00
    LinkedListNodeOverlay* next; // 0x04
};

static_assert(offsetof(LinkedListNodeOverlay, prev) == 0x00);
static_assert(offsetof(LinkedListNodeOverlay, next) == 0x04);

void __stdcall LinkedList_Unlink_and_Free(LinkedListNodeOverlay** list_ptr, LinkedListNodeOverlay* node)
{
    LinkedListNodeOverlay* const next = node->next;
    LinkedListNodeOverlay* const prev = node->prev;

    next->prev = prev;
    prev->next = next;
    std::free(node);
    *list_ptr = prev;
}