#include <cstdio>

struct Field14 { int a, b, c, d; };

static Field14* g_seen_arg = 0;
static int g_sub_called = 0;
static int g_base_called = 0;

struct BObj {
    int tag;
    void Sub(Field14* p);
};
void BObj::Sub(Field14* p) { g_sub_called = 1; g_seen_arg = p; }

struct AObj {
    char pad[0x70];
    BObj* field70;
};

struct BaseAction {
    void OnCreate();
};
void BaseAction::OnCreate() { g_base_called = 1; }

struct CTCActionUseBuyHouse {
    void* vtbl;
    AObj* field4;
    char pad[0x14 - 8];
    Field14 field14;
    void OnCreate();
};

void CTCActionUseBuyHouse::OnCreate()
{
    this->field4->field70->Sub(&this->field14);
    ((BaseAction*)this)->OnCreate();
}

int main()
{
    BObj b; b.tag = 7;
    AObj a; a.field70 = &b;
    CTCActionUseBuyHouse obj;
    obj.field4 = &a;
    obj.field14.a = 111;

    obj.OnCreate();

    bool ok = g_sub_called == 1
           && g_base_called == 1
           && g_seen_arg == &obj.field14
           && g_seen_arg->a == 111;

    if (ok) printf("BUYHOUSE_ONCREATE_OK\n");
    else    printf("FAIL sub=%d base=%d arg=%p\n", g_sub_called, g_base_called, (void*)g_seen_arg);
    return ok ? 0 : 1;
}