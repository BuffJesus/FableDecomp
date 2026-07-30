#include <stdio.h>

struct Node {
    Node* next;
    int   pad4;
    int   pad8;
    int   key;
    int   done;
};
struct CQuestManager {
    char  pad[0x74];
    Node* m_list;
};

// __fastcall: this=ecx, unused edx, objId on stack -> mirrors __fastcall + ret 4
void __fastcall SetObjectiveAsCompleted(CQuestManager* self, void* /*edx*/, int objId)
{
    Node* sentinel = self->m_list;
    Node* n = sentinel->next;
    if (n == sentinel) return;
    do {
        if (n->key == objId) { n->done = 1; return; }
        n = n->next;
    } while (n != sentinel);
}

typedef void (__fastcall *FN)(void* ecx, void* edx, int objId);

int main()
{
    Node sentinel;
    Node a, b, c;
    sentinel.next = &a;
    a.next = &b; b.next = &c; c.next = &sentinel;
    a.key = 10; a.done = 0;
    b.key = 20; b.done = 0;
    c.key = 30; c.done = 0;

    CQuestManager mgr;
    mgr.m_list = &sentinel;

    FN fn = (FN)&SetObjectiveAsCompleted;

    fn(&mgr, 0, 20);
    if (b.done != 1) { printf("FAIL b.done=%d\n", b.done); return 1; }
    if (a.done != 0 || c.done != 0) { printf("FAIL sibling touched\n"); return 1; }

    fn(&mgr, 0, 999);
    if (a.done != 0 || c.done != 0) { printf("FAIL missing changed\n"); return 1; }

    Node loopnode; loopnode.next = &loopnode;
    CQuestManager mgr2; mgr2.m_list = &loopnode;
    fn(&mgr2, 0, 5);

    printf("OK_0x004af960\n");
    return 0;
}