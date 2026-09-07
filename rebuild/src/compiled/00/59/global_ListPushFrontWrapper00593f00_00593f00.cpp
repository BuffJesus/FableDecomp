#pragma optimize("s",on)
struct ListIterator_00593f00{unsigned long node;__forceinline ListIterator_00593f00(const ListIterator_00593f00& other):node(other.node){}};class ListWrapper_00593f00{public:ListIterator_00593f00 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00593f00 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00593f00(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00593f00::ListPushFrontWrapper_00593f00(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)