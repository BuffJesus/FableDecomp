#pragma optimize("s",on)
struct LinkedListInsertNode
{
    LinkedListInsertNode* next;
    LinkedListInsertNode* prev;
};

LinkedListInsertNode* __stdcall LinkedList_AllocateNode(unsigned long value);

LinkedListInsertNode** __stdcall LinkedList_Insert_Before(
    LinkedListInsertNode** out_inserted,
    LinkedListInsertNode* node,
    unsigned long value)
{
    LinkedListInsertNode* inserted = LinkedList_AllocateNode(value);
    LinkedListInsertNode* previous = node->prev;
    inserted->next = node;
    inserted->prev = previous;
    previous->next = inserted;
    node->prev = inserted;
    *out_inserted = inserted;
    return out_inserted;
}