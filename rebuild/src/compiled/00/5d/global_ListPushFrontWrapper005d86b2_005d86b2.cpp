#pragma optimize("s",on)
struct ListIterator_005d86b2{unsigned long node;__forceinline ListIterator_005d86b2(const ListIterator_005d86b2& other):node(other.node){}};class ListWrapper_005d86b2{public:ListIterator_005d86b2 first;void InsertBefore(unsigned long* valueAddress,ListIterator_005d86b2 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_005d86b2(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_005d86b2::ListPushFrontWrapper_005d86b2(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)