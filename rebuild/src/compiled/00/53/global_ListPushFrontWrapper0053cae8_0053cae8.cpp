#pragma optimize("s",on)
struct ListIterator_0053cae8{unsigned long node;__forceinline ListIterator_0053cae8(const ListIterator_0053cae8& other):node(other.node){}};class ListWrapper_0053cae8{public:ListIterator_0053cae8 first;void InsertBefore(unsigned long* valueAddress,ListIterator_0053cae8 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_0053cae8(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_0053cae8::ListPushFrontWrapper_0053cae8(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)