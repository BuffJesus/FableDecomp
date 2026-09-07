#pragma optimize("s",on)
struct ListIterator_005939ee{unsigned long node;__forceinline ListIterator_005939ee(const ListIterator_005939ee& other):node(other.node){}};class ListWrapper_005939ee{public:ListIterator_005939ee first;void InsertBefore(unsigned long* valueAddress,ListIterator_005939ee firstNode,unsigned long valueCopy);void ListPushFrontWrapper_005939ee(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_005939ee::ListPushFrontWrapper_005939ee(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)