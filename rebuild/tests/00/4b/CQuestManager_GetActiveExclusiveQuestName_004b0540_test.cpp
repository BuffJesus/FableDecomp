#include <cstdio>

struct CCharString { int dummy; };

struct SubNode {
    int key; SubNode* next; char pad[0x28 - 8]; CCharString* name;
};
struct Sub {
    char base0; char pad0[3]; SubNode* end;
    SubNode* Find(int* pkey);
};
struct QData {
    char pad0[0x2c]; unsigned int flags; char pad1[0x44 - 0x2c - 4]; Sub sub;
};
struct TNode { TNode* left; char pad0[4]; char keybuf[1]; };
struct QManager { char pad0[0x5c]; TNode* treehdr; };

static QData* g_the_q;
static bool g_check_ret;

SubNode* Sub::Find(int*) { return 0; }
QData* my_QLookup(TNode*) { return g_the_q; }
bool my_SubCheck(SubNode*) { return g_check_ret; }

CCharString g_defaultQuestName;

CCharString* GetName(QManager* self)
{
    union { int scratch; SubNode* found; } u;
    u.scratch = 0;
    TNode* node = *(TNode**)self->treehdr;
    if (node != self->treehdr) {
        SubNode* found = u.found;
        do {
            QData* q = my_QLookup((TNode*)((char*)node + 8));
            if (q) {
                if (q->flags & 0x1000) {
                    Sub* sub = &q->sub;
                    u.scratch = 0x6c;
                    SubNode* r = sub->Find(&u.scratch);
                    SubNode* end = sub->end;
                    if (r == end || r->key > 0x6c) r = end;
                    found = r->next;
                }
                if (found) { if (my_SubCheck(found)) return (CCharString*)&found->name; }
            }
            node = node->left;
        } while (node != self->treehdr);
    }
    return &g_defaultQuestName;
}

int main() {
    // Case 1: empty tree -> default
    TNode hdr; hdr.left = &hdr;
    QManager m; m.treehdr = &hdr;
    g_the_q = 0;
    CCharString* r1 = GetName(&m);

    // Case 2: one node, QLookup returns q with no flag, found starts null-ish
    TNode n0; n0.left = &hdr;      // node -> then back to header
    hdr.left = &n0;                // header.left = first node
    static QData q; q.flags = 0;   // flag bit not set
    g_the_q = &q;
    g_check_ret = false;
    CCharString* r2 = GetName(&m); // found (garbage) may be nonzero but SubCheck false -> default
    // We cannot assert r2 precisely (found is uninitialized), but it must not crash.

    if (r1 == &g_defaultQuestName) {
        std::printf("CQuestManager_004b0540_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r1=%p def=%p\n", (void*)r1, (void*)&g_defaultQuestName);
    return 1;
}