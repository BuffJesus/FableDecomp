#pragma optimize("s",on)
struct ListIterator_006567ec{unsigned long node;__forceinline ListIterator_006567ec(const ListIterator_006567ec& other):node(other.node){}};class ListWrapper_006567ec{public:ListIterator_006567ec first;void InsertBefore(unsigned long* valueAddress,ListIterator_006567ec firstNode,unsigned long valueCopy);void ListPushFrontWrapper_006567ec(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_006567ec::ListPushFrontWrapper_006567ec(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)