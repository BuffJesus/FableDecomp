#include <cstdio>

static int g_order[3];
static int g_n;

struct MemberA { int a0,a4,a8,ac,a10; ~MemberA(); };
struct MemberB { int b0,b4,b8,bc,b10,b14,b18; ~MemberB(); };
struct MemberC { int c0,c4,c8; ~MemberC(); };

MemberA::~MemberA() { g_order[g_n++] = 1; }
MemberB::~MemberB() { g_order[g_n++] = 2; }
MemberC::~MemberC() { g_order[g_n++] = 3; }

struct Pair {
    MemberA a; MemberB b; MemberC c;
    ~Pair();
};
Pair::~Pair() {}

int main() {
    g_n = 0;
    {
        Pair p;
        (void)p;
    }
    // Reverse declared order: C(3), B(2), A(1)
    if (g_n == 3 && g_order[0]==3 && g_order[1]==2 && g_order[2]==1) {
        printf("DESTVAL_PAIR_OK\n");
        return 0;
    }
    printf("FAIL %d %d %d %d\n", g_n, g_order[0], g_order[1], g_order[2]);
    return 1;
}