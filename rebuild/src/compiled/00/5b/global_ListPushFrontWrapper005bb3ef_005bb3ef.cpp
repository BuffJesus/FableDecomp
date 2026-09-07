#pragma optimize("s",on)
struct ListIterator_005bb3ef{unsigned long node;__forceinline ListIterator_005bb3ef(const ListIterator_005bb3ef& other):node(other.node){}};class ListWrapper_005bb3ef{public:ListIterator_005bb3ef first;void InsertBefore(unsigned long* valueAddress,ListIterator_005bb3ef firstNode,unsigned long valueCopy);void ListPushFrontWrapper_005bb3ef(unsigned long value);};
#pragma optimize("y",off)
void ListWrapper_005bb3ef::ListPushFrontWrapper_005bb3ef(unsigned long value){InsertBefore(&value,first,value);}
#pragma optimize("y",on)