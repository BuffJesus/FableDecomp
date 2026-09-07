#pragma optimize("s",on)
struct ListIterator_005f8f6f{unsigned long node;__forceinline ListIterator_005f8f6f(const ListIterator_005f8f6f& other):node(other.node){}};class ListWrapper_005f8f6f{public:ListIterator_005f8f6f first;void InsertBefore(unsigned long* valueAddress,ListIterator_005f8f6f firstNode,unsigned long valueCopy);void ListPushFrontWrapper_005f8f6f(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_005f8f6f::ListPushFrontWrapper_005f8f6f(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)