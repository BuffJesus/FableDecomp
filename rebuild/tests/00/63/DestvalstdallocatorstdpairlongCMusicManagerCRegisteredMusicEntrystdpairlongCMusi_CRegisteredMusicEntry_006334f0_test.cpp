#include <cstdio>

struct Sub {
    int pad;
    void Helper();
};

struct Owner {
    char pad[0x140];
    Sub* m_a;
    char pad2[0x1a4 - 0x140 - 4];
    Sub* m_b;
    void _Dest_val_helper(void* al, void* pr);
};

static int g_calls = 0;
static Sub* g_seen[4];

void Sub::Helper()
{
    if (g_calls < 4) g_seen[g_calls] = this;
    g_calls++;
}

void Owner::_Dest_val_helper(void* al, void* pr)
{
    m_a->Helper();
    m_b->Helper();
}

int main()
{
    Owner o;
    Sub a, b;
    o.m_a = &a;
    o.m_b = &b;
    g_calls = 0;
    o._Dest_val_helper((void*)0x1234, (void*)0x5678);
    if (g_calls == 2 && g_seen[0] == &a && g_seen[1] == &b) {
        printf("DEST_VAL_OK\n");
        return 0;
    }
    printf("FAIL calls=%d\n", g_calls);
    return 1;
}