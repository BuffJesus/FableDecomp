#include <stdio.h>
#include <stdlib.h>

struct Marker { int id; unsigned char keep; };
struct QueryResult { char pad[0x91]; unsigned char flags; };
struct Node { Node* next; Node* prev; Marker m; };

static QueryResult g_qr;
static int g_freed;

// Retail semantics: keep node when query!=null AND (flags&1)==0 (bit CLEAR).
// keep==2 -> null result -> delete; keep==1 -> flags bit SET -> delete (dead);
// keep==0 -> flags bit CLEAR -> survive.
static QueryResult* __fastcall Marker_Query(Marker* self) {
    if (self->keep == 2) return 0;            // null -> delete
    g_qr.flags = (self->keep & 1) ? 1 : 0;    // bit set -> dead -> delete
    return &g_qr;
}
static void __fastcall Marker_Destroy(Marker* self) { (void)self; }
static void fable_free(void* p) { g_freed++; free(p); }

struct CMiniMapDisplay { char pad[0x20]; Node* head; };

static void __fastcall RemoveAllDeadMarkers(CMiniMapDisplay* self) {
    Node* head = self->head;
    Node* n = head->next;
    while (n != head) {
        Marker* m = &n->m;
        QueryResult* q = Marker_Query(m);
        if (q != 0 && (q->flags & 1) == 0) { n = n->next; }
        else {
            Node* nx = n->next; Node* pv = n->prev;
            pv->next = nx; nx->prev = pv;
            Marker_Destroy(m); fable_free(n); n = nx;
        }
    }
}

static Node* mk(unsigned char keep, int id) {
    Node* n = (Node*)malloc(sizeof(Node)); n->m.keep = keep; n->m.id = id; return n;
}

int main() {
    CMiniMapDisplay d; Node sentinel;
    d.head = &sentinel;
    // keep==0 survives; keep==1 (bit set, dead) deleted; keep==2 (null query) deleted.
    // sentinel <-> A(0,1) <-> B(1,2) <-> C(2,3) <-> D(0,4) <-> E(1,5) <-> sentinel
    // survivors: A and D. deleted+freed: B, C, E (3).
    Node* A = mk(0, 1); Node* B = mk(1, 2); Node* C = mk(2, 3); Node* D = mk(0, 4); Node* E = mk(1, 5);
    Node* order[7] = { &sentinel, A, B, C, D, E, &sentinel };
    for (int i = 0; i < 6; i++) { order[i]->next = order[i+1]; order[i+1]->prev = order[i]; }

    RemoveAllDeadMarkers(&d);

    int count = 0, ok = 1;
    Node* p = sentinel.next; int expect[2] = {1,4}; int idx = 0;
    while (p != &sentinel) { if (idx>=2 || p->m.id != expect[idx]) ok = 0; idx++; count++; p = p->next; }
    if (idx != 2) ok = 0;
    // reverse links intact
    p = sentinel.prev; int rexp[2] = {4,1}; idx = 0;
    while (p != &sentinel) { if (idx>=2 || p->m.id != rexp[idx]) ok = 0; idx++; p = p->prev; }
    if (g_freed != 3) ok = 0;

    if (ok) printf("PARITY_OK_82a2a0\n"); else printf("FAIL c=%d f=%d\n", count, g_freed);
    return 0;
}