#pragma optimize("s",on)
struct ListIterator_00660587{unsigned long node;__forceinline ListIterator_00660587(const ListIterator_00660587& other):node(other.node){}};class ListWrapper_00660587{public:ListIterator_00660587 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00660587 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00660587(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00660587::ListPushFrontWrapper_00660587(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)