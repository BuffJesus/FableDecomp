#include <cstdio>

struct Elem;
struct ElemVtbl {
    void (__fastcall *dtor)(Elem* self, int edx, int flag);
};
struct Elem {
    ElemVtbl* vptr;
    char pad[0x48 - 4];
};

void __fastcall _Destroy(Elem* first, Elem* last);

static int g_calls = 0;
static int g_flagSum = 0;
static Elem* g_seen[8];

static void __fastcall myDtor(Elem* self, int, int flag) {
    g_seen[g_calls] = self;
    g_flagSum += flag;
    g_calls++;
}

int main() {
    ElemVtbl vt;
    vt.dtor = myDtor;

    const int N = 5;
    Elem arr[N];
    for (int i = 0; i < N; ++i) arr[i].vptr = &vt;

    _Destroy(&arr[0], &arr[N]);

    bool ok = (g_calls == N) && (g_flagSum == 0);
    for (int i = 0; i < N && ok; ++i) {
        // stride 0x48 == sizeof(Elem)
        if (g_seen[i] != &arr[i]) ok = false;
    }
    // empty range
    g_calls = 0;
    _Destroy(&arr[0], &arr[0]);
    if (g_calls != 0) ok = false;

    if (ok) printf("DESTROY_OK\n");
    else printf("DESTROY_FAIL calls=%d\n", g_calls);
    return ok ? 0 : 1;
}