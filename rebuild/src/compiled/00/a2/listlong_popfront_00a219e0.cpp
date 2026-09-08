struct CListLongNode {
    CListLongNode* Next;
    CListLongNode* Previous;
    long Value;
};

void __cdecl DestroyListLongNode(CListLongNode* node);

struct CListLongRetail {
    CListLongNode* Sentinel;
    void pop_front();
};

void CListLongRetail::pop_front() {
    CListLongNode* node = Sentinel->Next;
    CListLongNode* next = node->Next;
    CListLongNode* previous = node->Previous;
    previous->Next = next;
    next->Previous = previous;
    DestroyListLongNode(node);
}