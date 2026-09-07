#pragma optimize("s",on)
struct ListIterator_0042b646{unsigned long node;__forceinline ListIterator_0042b646(const ListIterator_0042b646& other):node(other.node){}};class ListWrapper_0042b646{public:ListIterator_0042b646 first;void InsertBefore(unsigned long* valueAddress,ListIterator_0042b646 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_0042b646(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_0042b646::ListPushFrontWrapper_0042b646(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)