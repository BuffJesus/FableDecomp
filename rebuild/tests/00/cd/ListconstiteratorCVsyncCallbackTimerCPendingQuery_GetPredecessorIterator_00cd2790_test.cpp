#include <stdio.h>
struct _List_node_CPendingQuery;

struct _List_const_iterator_CPendingQuery {
    _List_node_CPendingQuery* _Ptr;
    _List_const_iterator_CPendingQuery GetPredecessorIterator() const;
};

struct _List_node_CPendingQuery {
    _List_node_CPendingQuery* _Next;
    void* _Value0;
    _List_node_CPendingQuery* _Prev;
};

_List_const_iterator_CPendingQuery
_List_const_iterator_CPendingQuery::GetPredecessorIterator() const {
    _List_const_iterator_CPendingQuery result;
    result._Ptr = _Ptr->_Prev;
    return result;
}

int main() {
    _List_node_CPendingQuery first;
    _List_node_CPendingQuery second;
    first._Next = &second;
    first._Value0 = 0;
    first._Prev = 0;
    second._Next = 0;
    second._Value0 = 0;
    second._Prev = &first;

    _List_const_iterator_CPendingQuery it;
    it._Ptr = &second;
    _List_const_iterator_CPendingQuery prev = it.GetPredecessorIterator();
    if (prev._Ptr != &first) return 1;
    if (it._Ptr != &second) return 2;   /* must not self-decrement */
    printf("PREDECESSOR_ITERATOR PASS\n");
    return 0;
}