#pragma optimize("s",on)
struct ListIterator_00655f62{unsigned long node;__forceinline ListIterator_00655f62(const ListIterator_00655f62& other):node(other.node){}};class ListWrapper_00655f62{public:ListIterator_00655f62 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00655f62 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00655f62(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00655f62::ListPushFrontWrapper_00655f62(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)