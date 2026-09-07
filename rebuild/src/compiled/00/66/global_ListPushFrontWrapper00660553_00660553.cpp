#pragma optimize("s",on)
struct ListIterator_00660553{unsigned long node;__forceinline ListIterator_00660553(const ListIterator_00660553& other):node(other.node){}};class ListWrapper_00660553{public:ListIterator_00660553 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00660553 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00660553(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00660553::ListPushFrontWrapper_00660553(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)