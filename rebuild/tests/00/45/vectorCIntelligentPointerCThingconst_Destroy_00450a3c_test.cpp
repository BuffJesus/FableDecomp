#include <cstdio>
#include <new>

static int g_calls = 0;
static char* g_last_this = 0;

struct Elem { virtual void vf0(int); };

void Elem::vf0(int a) { g_calls++; g_last_this = (char*)this; (void)a; }

void __fastcall vec_Destroy(Elem* first, Elem* last, int unused) {
    (void)unused;
    while (first != last) {
        first->vf0(0);
        first = (Elem*)((char*)first + 0x50);
    }
}

int main() {
    char buf[0x50 * 3];
    for (int i = 0; i < 3; i++) {
        // placement of vptr: construct Elem in each slot
        new (buf + i * 0x50) Elem();
    }
    Elem* first = (Elem*)buf;
    Elem* last = (Elem*)(buf + 0x50 * 3);
    vec_Destroy(first, last, 0);
    if (g_calls == 3 && g_last_this == buf + 0x50 * 2) {
        std::printf("vecdestroy_00450a3c_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", g_calls);
    return 1;
}