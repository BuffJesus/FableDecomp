#pragma optimize("s",on)
struct Node { Node* next; };
struct Sub { Node* head; };
extern void __fastcall detach_helper(Sub*);
struct S { char pad[0x40]; Sub sub; };
void __fastcall DetachTarget(S* self){ Sub* s = &self->sub; if(s->head->next != s->head) detach_helper(s); }