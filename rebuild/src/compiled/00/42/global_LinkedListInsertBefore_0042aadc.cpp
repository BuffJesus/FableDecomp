#pragma optimize("s",on)
struct LinkedListNode12
{
    LinkedListNode12* next;
    LinkedListNode12* prev;
    unsigned long payload;
};

LinkedListNode12* __stdcall LinkedList_AllocateNode_12bytes(unsigned long value);

LinkedListNode12** __stdcall LinkedList_InsertBefore(
    LinkedListNode12** output,
    LinkedListNode12* before,
    unsigned long value)
{
    LinkedListNode12* inserted = LinkedList_AllocateNode_12bytes(value);
    LinkedListNode12* previous = before->prev;
    inserted->next = before;
    inserted->prev = previous;
    previous->next = inserted;
    before->prev = inserted;
    *output = inserted;
    return output;
}