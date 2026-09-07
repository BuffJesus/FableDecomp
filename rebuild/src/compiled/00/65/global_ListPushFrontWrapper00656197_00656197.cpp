#pragma optimize("s",on)
struct ListIterator_00656197{unsigned long node;__forceinline ListIterator_00656197(const ListIterator_00656197& other):node(other.node){}};class ListWrapper_00656197{public:ListIterator_00656197 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00656197 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00656197(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00656197::ListPushFrontWrapper_00656197(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)