#include <cstdio>

struct Param { char pad[0x10]; int refcount; };

static int g_method_called = 0;
static int g_free_called = 0;

void method(Param* p) { (void)p; g_method_called++; }
void freep(Param* p) { (void)p; g_free_called++; }

void Render(Param* p) {
    if (--p->refcount == 0) {
        method(p);
        freep(p);
    }
}

int main() {
    Param a; a.refcount = 2;
    Render(&a);
    if (a.refcount != 1 || g_method_called != 0 || g_free_called != 0) { printf("FAIL1\n"); return 1; }
    Render(&a);
    if (a.refcount != 0 || g_method_called != 1 || g_free_called != 1) { printf("FAIL2\n"); return 1; }
    printf("RENDER_PARITY_OK\n");
    return 0;
}