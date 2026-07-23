#include <cstdio>
struct Node {
    void* pad0;
    void* data;
    Node* next;
    Node* prev;
};
struct CTCCarryable {
    Node* head;
    int count;
};
void* g_freed = (void*)0;
void __stdcall FreeNode(void* data){ g_freed = data; }
void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        Node* n = self->head;
        FreeNode(n->data);
        n = self->head; n->next = n;
        n = self->head; n->data = 0;
        n = self->head; n->prev = n;
        self->count = 0;
    }
}
int main(){
    Node node; node.pad0=(void*)1; node.data=(void*)0x1234; node.next=(Node*)7; node.prev=(Node*)9;
    CTCCarryable c; c.head=&node; c.count=5;
    CTCCarryable_OnKill(&c);
    if (c.count!=0){ std::printf("FAIL count\n"); return 1; }
    if (g_freed!=(void*)0x1234){ std::printf("FAIL freed\n"); return 1; }
    if (node.next!=&node || node.prev!=&node){ std::printf("FAIL links\n"); return 1; }
    if (node.data!=(void*)0){ std::printf("FAIL data\n"); return 1; }
    std::printf("CTCCarryable_00453607_TEST PASS\n");
    return 0;
}