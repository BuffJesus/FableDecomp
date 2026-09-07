#pragma optimize("s",on)
struct ListIterator_006561c8{unsigned long node;__forceinline ListIterator_006561c8(const ListIterator_006561c8& other):node(other.node){}};class ListWrapper_006561c8{public:ListIterator_006561c8 first;void InsertBefore(unsigned long* valueAddress,ListIterator_006561c8 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_006561c8(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_006561c8::ListPushFrontWrapper_006561c8(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)