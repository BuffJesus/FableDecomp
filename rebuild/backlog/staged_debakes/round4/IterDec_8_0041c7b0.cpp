// list iterator retreat: `this->node = this->node->prev;`. __fastcall this=ecx.
struct Node { Node* next; Node* prev; };
struct Iter { Node* node; void Retreat(); };
void Iter::Retreat() { this->node = this->node->prev; }
