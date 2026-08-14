#include <cstdio>

struct Obj {
    unsigned long a;
    unsigned long b;
};

int g_helper_called = 0;
int helper() { g_helper_called = 1; return 42; }
int store(Obj* self, int arg) { return arg + 1; }

void reset(Obj* self) {
    self->a = 0;
    self->b = store(self, helper());
}

int main() {
    Obj o;
    o.a = 0xDEADBEEF;
    o.b = 0;
    reset(&o);
    if (o.a == 0 && g_helper_called == 1 && o.b == 43) {
        printf("BEHAVIOUR_OK\n");
    } else {
        printf("BEHAVIOUR_FAIL a=%lu b=%lu h=%d\n", o.a, o.b, g_helper_called);
    }
    return 0;
}