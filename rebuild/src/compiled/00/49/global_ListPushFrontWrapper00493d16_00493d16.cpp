#pragma optimize("s",on)
struct ListIterator_00493d16{unsigned long node;__forceinline ListIterator_00493d16(const ListIterator_00493d16& other):node(other.node){}};class ListWrapper_00493d16{public:ListIterator_00493d16 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00493d16 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00493d16(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00493d16::ListPushFrontWrapper_00493d16(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)