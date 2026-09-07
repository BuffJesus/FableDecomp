#pragma optimize("s",on)
struct ListIterator_005f8f50{unsigned long node;__forceinline ListIterator_005f8f50(const ListIterator_005f8f50& other):node(other.node){}};class ListWrapper_005f8f50{public:ListIterator_005f8f50 first;void InsertBefore(unsigned long* valueAddress,ListIterator_005f8f50 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_005f8f50(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_005f8f50::ListPushFrontWrapper_005f8f50(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)