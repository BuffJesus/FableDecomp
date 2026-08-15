#include <cstdio>

struct Renderer {
    char *cur;
    char *pad4;
    char *end;
};

static int g_assign = 0;
static int g_grow = 0;

void SlotAssign(char *slot) { (void)slot; g_assign++; }
void Grow(Renderer *self) { (void)self; g_grow++; }

void AddPrimitive(Renderer *self) {
    char *e = self->end;
    char *c = self->cur;
    if (c != e - 8) {
        SlotAssign(c);
        self->cur += 8;
        return;
    }
    Grow(self);
}

int main() {
    char buf[64];
    Renderer r;
    r.cur = buf;
    r.end = buf + 40; // cur != end-8 -> assign path
    AddPrimitive(&r);
    bool ok1 = (g_assign == 1 && g_grow == 0 && r.cur == buf + 8);

    // now cur == end-8 -> grow path
    r.cur = buf + 32;
    r.end = buf + 40;
    AddPrimitive(&r);
    bool ok2 = (g_grow == 1 && g_assign == 1 && r.cur == buf + 32);

    if (ok1 && ok2) printf("BEHAVIOR_OK\n");
    else printf("BEHAVIOR_FAIL a=%d g=%d\n", g_assign, g_grow);
    return 0;
}