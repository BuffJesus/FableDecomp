#include <cstdio>

static int g_dtor = 0;
static int g_freed = 0;

struct CTCMapwho {
    int dummy;
    void dtor() { dummy = 0; g_dtor++; }
};

static void my_delete(void* p) { if (p) g_freed++; }

void OnDieModel(CTCMapwho* self)
{
    if (self) {
        self->dtor();
        my_delete(self);
    }
}

int main()
{
    OnDieModel(0);
    if (g_dtor != 0 || g_freed != 0) { printf("FAIL null\n"); return 1; }

    CTCMapwho obj; obj.dummy = 5;
    OnDieModel(&obj);
    if (g_dtor != 1 || g_freed != 1) { printf("FAIL nonnull\n"); return 1; }

    printf("OK_00668ed0\n");
    return 0;
}