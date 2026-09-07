#pragma optimize("s",on)
struct LinkedListIterator
{
    unsigned long node;
    __forceinline LinkedListIterator(const LinkedListIterator& other)
        : node(other.node) {}
};

class LinkedList
{
public:
    LinkedListIterator first;
    void InsertBefore(
        unsigned long* valueAddress,
        LinkedListIterator firstNode,
        unsigned long valueCopy);
    void LinkedList_PushFront(unsigned long value);
};
#pragma optimize("y", off)
void LinkedList::LinkedList_PushFront(unsigned long value)
{
    InsertBefore(&value, first, value);
}
#pragma optimize("y", on)