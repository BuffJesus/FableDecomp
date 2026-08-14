#include <cstdio>
typedef unsigned long DWORD;

struct Node { Node* _00; char pad[0x14]; DWORD _18; };
struct Sub { Node* head; };
struct CStopWatch { char pad[0x58]; Sub sub; };

static Node* g_result;
static Node* Model_Find(Sub*, void*) { return g_result; }

static DWORD GetTicks_model(CStopWatch* self, int a1, int a2)
{
    Sub* s = &self->sub;
    Node* r = Model_Find(s, &a1);
    if (r == s->head) return 0;
    return r->_18;
}

int main()
{
    CStopWatch w;
    Node endSentinel;
    Node found;
    found._18 = 0x12345678u;
    w.sub.head = &endSentinel;

    g_result = &endSentinel;
    DWORD a = GetTicks_model(&w, 0, 0);

    g_result = &found;
    DWORD b = GetTicks_model(&w, 0, 0);

    if (a == 0 && b == 0x12345678u)
        printf("PARITY_OK\n");
    else
        printf("FAIL a=%lu b=%lu\n", a, b);
    return 0;
}