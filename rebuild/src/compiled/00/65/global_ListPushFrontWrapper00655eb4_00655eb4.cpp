#pragma optimize("s",on)
struct ListIterator_00655eb4{unsigned long node;__forceinline ListIterator_00655eb4(const ListIterator_00655eb4& other):node(other.node){}};class ListWrapper_00655eb4{public:ListIterator_00655eb4 first;void InsertBefore(unsigned long* valueAddress,ListIterator_00655eb4 firstNode,unsigned long valueCopy);void ListPushFrontWrapper_00655eb4(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_00655eb4::ListPushFrontWrapper_00655eb4(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)