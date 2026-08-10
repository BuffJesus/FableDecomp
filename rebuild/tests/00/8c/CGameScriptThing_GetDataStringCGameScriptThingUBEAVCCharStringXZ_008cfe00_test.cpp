#include <cstdio>

struct Arg { void* got; };
struct CGameScriptThing { char pad[0x24]; char data; };

static void model_consume(Arg* a, void* p) { a->got = p; }

void* model_GetDataString(CGameScriptThing* self, void* arg) {
    model_consume((Arg*)arg, &self->data);
    return arg;
}

int main() {
    CGameScriptThing t;
    Arg a; a.got = 0;
    void* r = model_GetDataString(&t, &a);
    if (r != &a) { printf("FAIL ret\n"); return 1; }
    if (a.got != &t.data) { printf("FAIL arg\n"); return 1; }
    Arg b; b.got = (void*)1;
    void* r2 = model_GetDataString(&t, &b);
    if (r2 != &b || b.got != &t.data) { printf("FAIL2\n"); return 1; }
    printf("OK_008cfe00\n");
    return 0;
}