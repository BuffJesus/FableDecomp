#pragma optimize("s",on)
struct ListIterator_0057955a{unsigned long node;__forceinline ListIterator_0057955a(const ListIterator_0057955a& other):node(other.node){}};class ListWrapper_0057955a{public:ListIterator_0057955a first;void InsertBefore(unsigned long* valueAddress,ListIterator_0057955a firstNode,unsigned long valueCopy);void ListPushFrontWrapper_0057955a(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_0057955a::ListPushFrontWrapper_0057955a(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)