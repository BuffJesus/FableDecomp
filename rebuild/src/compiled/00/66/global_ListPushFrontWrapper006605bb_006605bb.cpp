#pragma optimize("s",on)
struct ListIterator_006605bb{unsigned long node;__forceinline ListIterator_006605bb(const ListIterator_006605bb& other):node(other.node){}};class ListWrapper_006605bb{public:ListIterator_006605bb first;void InsertBefore(unsigned long* valueAddress,ListIterator_006605bb firstNode,unsigned long valueCopy);void ListPushFrontWrapper_006605bb(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_006605bb::ListPushFrontWrapper_006605bb(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)