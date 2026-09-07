#pragma optimize("s",on)
struct ListIterator_0066051f{unsigned long node;__forceinline ListIterator_0066051f(const ListIterator_0066051f& other):node(other.node){}};class ListWrapper_0066051f{public:ListIterator_0066051f first;void InsertBefore(unsigned long* valueAddress,ListIterator_0066051f firstNode,unsigned long valueCopy);void ListPushFrontWrapper_0066051f(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_0066051f::ListPushFrontWrapper_0066051f(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)