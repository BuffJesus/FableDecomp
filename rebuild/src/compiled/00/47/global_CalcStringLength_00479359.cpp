#pragma optimize("s",on)
struct Node { Node* next; };
struct List {
    Node* head;
    int CalcStringLength();
};
int List::CalcStringLength() {
    Node* sentinel = head;
    Node* p = sentinel->next;
    int n = 0;
    while (p != sentinel) {
        p = p->next;
        ++n;
    }
    return n;
}