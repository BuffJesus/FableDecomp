#include <cstdio>
struct Node {
    Node* next;
    Node* prev;
    char pad[0x10];
    int sub;
};
struct CGuiControlTreePane {
    Node* head;
};
static int g_dtor_calls = 0;
static int g_free_calls = 0;
extern "C" void __fastcall NodeSub_Dtor(int* self) { (void)self; ++g_dtor_calls; }
extern "C" void FreeNode(Node* n) { (void)n; ++g_free_calls; }

void __fastcall CGuiControlTreePane_SortTreeRecursively(CGuiControlTreePane* self)
{
    Node* head = self->head;
    Node* cur = head->next;
    if (cur != head) {
        do {
            Node* node = cur;
            cur = cur->next;
            NodeSub_Dtor(&node->sub);
            FreeNode(node);
        } while (cur != self->head);
    }
    head = self->head;
    head->next = head;
    head = self->head;
    head->prev = head;
}

int main() {
    // Build circular list: head <-> a <-> b <-> head
    static Node head, a, b;
    head.next = &a; a.prev = &head;
    a.next = &b;    b.prev = &a;
    b.next = &head; head.prev = &b;
    CGuiControlTreePane pane; pane.head = &head;
    CGuiControlTreePane_SortTreeRecursively(&pane);
    if (g_dtor_calls != 2) { std::printf("FAIL dtor=%d\n", g_dtor_calls); return 1; }
    if (g_free_calls != 2) { std::printf("FAIL free=%d\n", g_free_calls); return 1; }
    if (head.next != &head || head.prev != &head) { std::printf("FAIL reset\n"); return 1; }
    std::printf("CGuiControlTreePane_004407b0_TEST PASS\n");
    return 0;
}