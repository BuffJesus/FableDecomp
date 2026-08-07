// Front-end history deque iterator node binding @ 0x0059A959.
//
// Iterator layout is current (+0), first (+4), last (+8), node (+0x0C).
// Binding a map node derives the 32-pointer block bounds while deliberately
// leaving current unchanged.

struct FrontEndHistoryDequeIterator_0059a959
{
    void** current;
    void** first;
    void** last;
    void*** node;

    void SetNode(void*** new_node);
};

void FrontEndHistoryDequeIterator_0059a959::SetNode(void*** new_node)
{
    node = new_node;
    first = *new_node;
    last = first + 0x20;
}
