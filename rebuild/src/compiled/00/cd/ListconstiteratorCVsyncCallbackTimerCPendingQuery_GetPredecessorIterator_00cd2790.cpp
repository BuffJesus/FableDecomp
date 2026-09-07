// Retail 0x00CD2790. The manifest names this row `operator--`, but the body never
// writes back to `this` - it only builds a fresh iterator over the node link at +8 and
// returns it through the hidden sret slot (`ret 4` with no visible argument). Modelled
// mechanically as that: a const accessor returning the predecessor iterator by value.
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