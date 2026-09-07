#pragma optimize("s",on)
struct ListIterator_00660505{unsigned long node;__forceinline ListIterator_00660505(const ListIterator_00660505& other):node(other.node){}};class ListWrapper_00660505{public:ListIterator_00660505 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00660505 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00660505(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00660505::ListPushFrontWrapper_00660505(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)