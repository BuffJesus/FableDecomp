#pragma optimize("s",on)
struct ListIterator_006604eb{unsigned long node;__forceinline ListIterator_006604eb(const ListIterator_006604eb& other):node(other.node){}};class ListWrapper_006604eb{public:ListIterator_006604eb first;void InsertBefore(unsigned long* valueAddress,ListIterator_006604eb firstNode,unsigned long valueCopy);void ListPushFrontWrapper_006604eb(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_006604eb::ListPushFrontWrapper_006604eb(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)