#pragma optimize("s",on)
struct ListIterator_00660fc9{unsigned long node;__forceinline ListIterator_00660fc9(const ListIterator_00660fc9& other):node(other.node){}};class ListWrapper_00660fc9{public:ListIterator_00660fc9 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00660fc9 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00660fc9(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00660fc9::ListPushFrontWrapper_00660fc9(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)