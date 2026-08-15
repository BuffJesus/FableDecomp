#include <cstdio>

struct Renderer { char* cur; char* pad; char* end; };

static int g_init = 0;
static int g_grew = 0;
static char* g_last_init = 0;

static void InitPrimitiveSlot(void* slot) { g_init++; g_last_init = (char*)slot; }
static void GrowAndAddPrimitive(Renderer* self) { g_grew++; (void)self; }

static void AddPrimitive(Renderer* self) {
    if (self->cur != self->end - 0x1c) {
        InitPrimitiveSlot(self->cur);
        self->cur += 0x1c;
        return;
    }
    GrowAndAddPrimitive(self);
}

int main() {
    char buf[0x1c * 4];
    Renderer r;
    r.cur = buf;
    r.end = buf + 0x1c * 2;   // full when cur == end - 0x1c

    // first add: cur(buf) != end-0x1c(buf+0x1c) -> init slot at buf, advance
    AddPrimitive(&r);
    if (g_init != 1 || g_grew != 0) { printf("FAIL a\n"); return 1; }
    if (g_last_init != buf) { printf("FAIL b\n"); return 1; }
    if (r.cur != buf + 0x1c) { printf("FAIL c\n"); return 1; }

    // second add: cur(buf+0x1c) == end-0x1c -> grow
    AddPrimitive(&r);
    if (g_init != 1 || g_grew != 1) { printf("FAIL d\n"); return 1; }
    if (r.cur != buf + 0x1c) { printf("FAIL e\n"); return 1; }

    printf("PARITY_OK_493cf5\n");
    return 0;
}