#include <stdio.h>

struct CActiveFile { void* p0; int base_called; int freed; };

static void base_ORF(CActiveFile* self){ self->base_called = 1; }
static void free_thing(void* p){ (void)p; }

static void OnReadFinished_6_model(CActiveFile* self, int* freed_flag)
{
    base_ORF(self);
    void* p = self->p0;
    if (p != 0) { free_thing(p); *freed_flag = 1; }
}

int main()
{
    // branch: p0 != 0 -> free
    int freed = 0;
    int dummy = 42;
    CActiveFile a; a.p0 = &dummy; a.base_called = 0; a.freed = 0;
    OnReadFinished_6_model(&a, &freed);
    if (!a.base_called) { printf("FAIL base\n"); return 1; }
    if (!freed) { printf("FAIL free\n"); return 1; }

    // branch: p0 == 0 -> no free
    freed = 0;
    CActiveFile b; b.p0 = 0; b.base_called = 0; b.freed = 0;
    OnReadFinished_6_model(&b, &freed);
    if (!b.base_called) { printf("FAIL base2\n"); return 1; }
    if (freed) { printf("FAIL nofree\n"); return 1; }

    printf("OK_004dd32b\n");
    return 0;
}