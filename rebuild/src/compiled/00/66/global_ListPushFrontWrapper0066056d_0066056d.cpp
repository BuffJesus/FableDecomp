#pragma optimize("s",on)
struct ListIterator_0066056d{unsigned long node;__forceinline ListIterator_0066056d(const ListIterator_0066056d& other):node(other.node){}};class ListWrapper_0066056d{public:ListIterator_0066056d first;void InsertBefore(unsigned long* valueAddress,ListIterator_0066056d firstNode,unsigned long valueCopy);void ListPushFrontWrapper_0066056d(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_0066056d::ListPushFrontWrapper_0066056d(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)