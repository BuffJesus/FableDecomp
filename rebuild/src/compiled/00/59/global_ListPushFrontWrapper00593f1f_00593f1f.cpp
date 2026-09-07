#pragma optimize("s",on)
struct ListIterator_00593f1f{unsigned long node;__forceinline ListIterator_00593f1f(const ListIterator_00593f1f& other):node(other.node){}};class ListWrapper_00593f1f{public:ListIterator_00593f1f first;void InsertBefore(unsigned long* valueAddress,ListIterator_00593f1f firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00593f1f(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00593f1f::ListPushFrontWrapper_00593f1f(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)