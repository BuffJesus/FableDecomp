// list iterator advance: `this->node = this->node->next;`. __fastcall this=ecx.
struct Node { Node* next; Node* prev; };
struct Iter { Node* node; void Advance(); };
void Iter::Advance() { this->node = this->node->next; }
