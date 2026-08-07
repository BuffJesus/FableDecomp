#include <cstdio>
struct Node { Node* next; };
struct Sub { Node* head; };
static int g=0; static void __fastcall detach_helper(Sub* s){ (void)s; g=1; }
struct S { char pad[0x40]; Sub sub; };
static void DetachTarget(S* self){ Sub* s=&self->sub; if(s->head->next!=s->head) detach_helper(s); }
int main(){ Node n; n.next=&n; S o; o.sub.head=&n; DetachTarget(&o); if(g){ std::printf("006463f0_TEST FAIL\n"); return 1;}
 Node a,b; a.next=&b; o.sub.head=&a; DetachTarget(&o); if(!g){ std::printf("006463f0_TEST FAIL\n"); return 1;}
 std::printf("006463f0_TEST PASS\n"); return 0;}