#pragma optimize("s",on)
struct ListIterator_006605a1{unsigned long node;__forceinline ListIterator_006605a1(const ListIterator_006605a1& other):node(other.node){}};class ListWrapper_006605a1{public:ListIterator_006605a1 first;void InsertBefore(unsigned long* valueAddress,ListIterator_006605a1 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_006605a1(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_006605a1::ListPushFrontWrapper_006605a1(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)