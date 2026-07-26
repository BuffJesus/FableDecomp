#include <cstdio>

struct IPrimitive {
    struct VT { void (__fastcall *dtor)(IPrimitive*); void (__fastcall *slot1)(IPrimitive*); };
    VT* vt;
};

struct ListNode {
    ListNode* next;
    ListNode* prev;
    IPrimitive* payload;
};

struct SubObj;
static int g_subCleared = 0;
static int g_counter = 100;

void __fastcall SubObj_Clear(SubObj* s) { (void)s; g_subCleared = 1; }
int __cdecl GetSomeCounter() { return g_counter; }

struct CTCGraphicAppearance {
    char pad[0x38];
    SubObj* subptr;
    ListNode* head;
    char pad2[0x54 - 0x40];
    int counter;
    char flags;
    char state;
};

void __fastcall CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRange(CTCGraphicAppearance* self)
{
    if (self->state < 0) {
        ListNode* sentinel = self->head;
        ListNode* n = sentinel->next;
        while (n != sentinel) {
            IPrimitive* p = n->payload;
            p->vt->slot1(p);
            n = n->next;
        }
        SubObj_Clear((SubObj*)((char*)self + 0x38));
        self->counter = GetSomeCounter() + 1;
        self->state &= 0x7f;
    }
}

static int g_primCalls = 0;
void __fastcall prim_slot1(IPrimitive* p) { (void)p; g_primCalls++; }

int main() {
    IPrimitive::VT vt; vt.dtor = 0; vt.slot1 = prim_slot1;
    IPrimitive p1; p1.vt = &vt;
    IPrimitive p2; p2.vt = &vt;

    ListNode sentinel;
    ListNode n1, n2;
    n1.payload = &p1; n2.payload = &p2;
    sentinel.next = &n1; n1.next = &n2; n2.next = &sentinel;

    CTCGraphicAppearance obj;
    for (int i = 0; i < (int)sizeof(obj); i++) ((char*)&obj)[i] = 0;
    obj.head = &sentinel;
    obj.counter = 0;
    obj.state = (char)0x80;  // negative -> triggers

    CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRange(&obj);

    if (g_primCalls == 2 && g_subCleared == 1 && obj.counter == 101 && (obj.state & 0x80) == 0) {
        // test the non-triggering path
        g_primCalls = 0; g_subCleared = 0;
        obj.state = 0x10; obj.counter = 55;
        CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRange(&obj);
        if (g_primCalls == 0 && g_subCleared == 0 && obj.counter == 55) {
            std::printf("CTCGraphicAppearance_004bf960_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL calls=%d sub=%d counter=%d state=%d\n", g_primCalls, g_subCleared, obj.counter, (int)obj.state);
    return 1;
}