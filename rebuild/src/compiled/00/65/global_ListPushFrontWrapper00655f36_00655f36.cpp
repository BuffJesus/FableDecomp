#pragma optimize("s",on)
struct ListIterator_00655f36{unsigned long node;__forceinline ListIterator_00655f36(const ListIterator_00655f36& other):node(other.node){}};class ListWrapper_00655f36{public:ListIterator_00655f36 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00655f36 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00655f36(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00655f36::ListPushFrontWrapper_00655f36(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)