// List iterator step: `this->node = this->node->next;`. __fastcall this=ecx.
#pragma pack(push,1)
struct Node {
    Node* link;
};
struct Iter { Node* node; void Step(); };
#pragma pack(pop)
void Iter::Step() { this->node = this->node->link; }