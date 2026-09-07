#pragma optimize("s",on)
struct ListIterator_005805c9{unsigned long node;__forceinline ListIterator_005805c9(const ListIterator_005805c9& other):node(other.node){}};class ListWrapper_005805c9{public:ListIterator_005805c9 first;void InsertBefore(unsigned long* valueAddress,ListIterator_005805c9 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_005805c9(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_005805c9::ListPushFrontWrapper_005805c9(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)