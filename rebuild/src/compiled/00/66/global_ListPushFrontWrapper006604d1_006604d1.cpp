#pragma optimize("s",on)
struct ListIterator_006604d1{unsigned long node;__forceinline ListIterator_006604d1(const ListIterator_006604d1& other):node(other.node){}};class ListWrapper_006604d1{public:ListIterator_006604d1 first;void InsertBefore(unsigned long* valueAddress,ListIterator_006604d1 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_006604d1(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_006604d1::ListPushFrontWrapper_006604d1(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)