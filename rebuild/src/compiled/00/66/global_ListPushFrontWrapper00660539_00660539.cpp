#pragma optimize("s",on)
struct ListIterator_00660539{unsigned long node;__forceinline ListIterator_00660539(const ListIterator_00660539& other):node(other.node){}};class ListWrapper_00660539{public:ListIterator_00660539 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00660539 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00660539(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00660539::ListPushFrontWrapper_00660539(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)