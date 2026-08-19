// List iterator step: `this->node = this->node->prev;`. __fastcall this=ecx.
#pragma pack(push,1)
struct Node {
    char pad[0x4];
    Node* link;
};
struct Iter { Node* node; void Step(); };
#pragma pack(pop)
void Iter::Step() { this->node = this->node->link; }