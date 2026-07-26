#include <cstdio>

static int g_calls = 0;
static int g_lastArg = -1;
struct Elem {
    void** vt;
    char pad[0x5c - 4];
};
typedef void (__fastcall *DtorFn)(Elem*, int);
static void __fastcall my_dtor(Elem* e, int a) { (void)e; g_calls++; g_lastArg = a; }

void __fastcall vec_Destroy(Elem* first, Elem* last, void* al)
{
    (void)al;
    for (; first != last; first = (Elem*)((char*)first + 0x5c)) {
        DtorFn f = (DtorFn)first->vt[0];
        f(first, 0);
    }
}

int main()
{
    const int N = 3;
    static char buf[0x5c * N];
    void* vtbl[1]; vtbl[0] = (void*)&my_dtor;
    for (int i = 0; i < N; i++) {
        Elem* e = (Elem*)(buf + i * 0x5c);
        e->vt = vtbl;
    }
    Elem* first = (Elem*)buf;
    Elem* last = (Elem*)(buf + 0x5c * N);
    vec_Destroy(first, last, 0);
    if (g_calls == N && g_lastArg == 0) {
        std::printf("vecCIP_00450a58_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d arg=%d\n", g_calls, g_lastArg);
    return 1;
}