#pragma optimize("s",on)
struct ListIterator_0058056f{unsigned long node;__forceinline ListIterator_0058056f(const ListIterator_0058056f& other):node(other.node){}};class ListWrapper_0058056f{public:ListIterator_0058056f first;void InsertBefore(unsigned long* valueAddress,ListIterator_0058056f firstNode,unsigned long valueCopy);void ListPushFrontWrapper_0058056f(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_0058056f::ListPushFrontWrapper_0058056f(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)