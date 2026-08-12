#include <cstdio>

struct Obj
{
    int cleaned;
    void Cleanup();
};

void FreeObj(Obj* p);

struct CTCMapwho
{
    void OnDie(Obj* p);
};

static int g_cleanCount = 0;
static int g_freeCount  = 0;

void Obj::Cleanup()
{
    this->cleaned = 1;
    g_cleanCount++;
}

void FreeObj(Obj* p)
{
    if (p) g_freeCount++;
}

void CTCMapwho::OnDie(Obj* p)
{
    if (p)
    {
        p->Cleanup();
        FreeObj(p);
    }
}

int main()
{
    CTCMapwho m;

    // null arg: nothing happens
    m.OnDie(0);
    if (g_cleanCount != 0 || g_freeCount != 0) { printf("FAIL null\n"); return 1; }

    // non-null: cleanup + free once
    Obj o; o.cleaned = 0;
    m.OnDie(&o);
    if (o.cleaned != 1 || g_cleanCount != 1 || g_freeCount != 1) { printf("FAIL nonnull\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}