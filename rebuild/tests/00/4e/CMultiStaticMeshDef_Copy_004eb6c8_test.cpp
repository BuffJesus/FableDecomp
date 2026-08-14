#include <cstdio>

struct SubC { int tag; int copied_from; };
struct Base { char pad[0x28]; SubC sub; };
struct Def  { char pad[0x28]; SubC sub; };

static int g_base_calls = 0;

void baseCopy(Base* self, const Base* other) {
    g_base_calls++;
    (void)self; (void)other;
}
void subCopy(SubC* self, const SubC* other) {
    self->copied_from = other->tag;
}

void modelCopy(Def* self, const Base* other) {
    baseCopy((Base*)self, other);
    subCopy(&self->sub, (const SubC*)((const char*)other + 0x28));
}

int main() {
    Base src; for (int i=0;i<(int)sizeof(src);++i) ((char*)&src)[i]=0;
    src.sub.tag = 12345;
    Def dst; for (int i=0;i<(int)sizeof(dst);++i) ((char*)&dst)[i]=0;

    modelCopy(&dst, &src);

    if (g_base_calls == 1 && dst.sub.copied_from == 12345) {
        printf("COPY_OK\n");
    } else {
        printf("COPY_FAIL %d %d\n", g_base_calls, dst.sub.copied_from);
    }
    return 0;
}