#include <cstdio>

struct C3DVector { float x, y, z; };

struct Inner;
struct CTCCreatureNavigation { void* field_0; Inner* field_4; };

struct Inner {
    int calls;
    CTCCreatureNavigation* last_self;
    C3DVector* last_dest;
    bool ret;
    bool Check(CTCCreatureNavigation* self, C3DVector* dest) {
        calls++; last_self = self; last_dest = dest; return ret;
    }
};

static bool model(CTCCreatureNavigation* self, C3DVector* dest) {
    return self->field_4->Check(self, dest);
}

int main() {
    Inner inner; inner.calls = 0; inner.ret = true; inner.last_self = 0; inner.last_dest = 0;
    CTCCreatureNavigation nav; nav.field_0 = 0; nav.field_4 = &inner;
    C3DVector d; d.x = 1; d.y = 2; d.z = 3;

    bool r = model(&nav, &d);
    if (!r) { printf("FAIL ret\n"); return 1; }
    if (inner.calls != 1) { printf("FAIL calls\n"); return 1; }
    if (inner.last_self != &nav) { printf("FAIL self\n"); return 1; }
    if (inner.last_dest != &d) { printf("FAIL dest\n"); return 1; }

    inner.ret = false;
    bool r2 = model(&nav, &d);
    if (r2) { printf("FAIL ret2\n"); return 1; }
    if (inner.calls != 2) { printf("FAIL calls2\n"); return 1; }

    printf("OK_00c0f3e0\n");
    return 0;
}