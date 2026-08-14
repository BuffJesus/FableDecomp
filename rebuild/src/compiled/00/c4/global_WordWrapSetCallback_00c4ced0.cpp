struct Node {
    void* cb;      // +0
    Node* prev;    // +4
    Node* next;    // +8
    void WordWrap_SetCallback(Node* a0, Node* a1);
};

void Node::WordWrap_SetCallback(Node* a0, Node* a1)
{
    if (a0)
        a0->next = this;
    if (a1)
        a1->prev = this;
    this->prev = a0;
    this->next = a1;
}