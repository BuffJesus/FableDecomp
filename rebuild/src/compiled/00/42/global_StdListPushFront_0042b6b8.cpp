#pragma optimize("s",on)
struct StdListIterator
{
    void* node;
    __forceinline StdListIterator(const StdListIterator& other)
        : node(other.node) {}
};

class StdList
{
public:
    StdListIterator first;
    void Insert(
        void** valueAddress,
        StdListIterator firstNode,
        void* valueCopy);
    void Std_List_PushFront(void* value);
};
#pragma optimize("y", off)
void StdList::Std_List_PushFront(void* value)
{
    Insert(&value, first, value);
}
#pragma optimize("y", on)