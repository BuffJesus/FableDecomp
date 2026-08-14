#include <cstdio>

struct Node { void* left; char pad4[0x14]; unsigned int value; };
struct Map  { Node* head; };
struct CStopWatch { char pad[0x58]; Map themap; };

static Node* g_ret;
static Node* Find(Map* self, void* iterOut) { (void)self; (void)iterOut; return g_ret; }

static unsigned int GetTicks(CStopWatch* self, __int64 key) {
    (void)key;
    Map* m = &self->themap;
    Node* n = Find(m, &key);
    if (n != m->head)
        return n->value;
    return 0;
}

int main() {
    CStopWatch sw;
    sw.themap.head = (Node*)0xDEAD;

    Node found;
    found.value = 12345u;

    // case 1: find returns head sentinel -> 0
    g_ret = sw.themap.head;
    unsigned int r1 = GetTicks(&sw, 0);

    // case 2: find returns a real node -> node->value
    g_ret = &found;
    unsigned int r2 = GetTicks(&sw, 0);

    if (r1 == 0u && r2 == 12345u) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL %u %u\n", r1, r2);
    return 0;
}