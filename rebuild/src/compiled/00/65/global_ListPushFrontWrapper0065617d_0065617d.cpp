#pragma optimize("s",on)
struct ListIterator_0065617d{unsigned long node;__forceinline ListIterator_0065617d(const ListIterator_0065617d& other):node(other.node){}};class ListWrapper_0065617d{public:ListIterator_0065617d first;void InsertBefore(unsigned long* valueAddress,ListIterator_0065617d firstNode,unsigned long valueCopy);void ListPushFrontWrapper_0065617d(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_0065617d::ListPushFrontWrapper_0065617d(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)