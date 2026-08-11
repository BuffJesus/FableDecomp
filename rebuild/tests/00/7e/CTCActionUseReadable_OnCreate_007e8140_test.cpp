#include <cstdio>

struct Sub;
struct Consumer;

struct Consumer {
    void Consume(int* arg);
};
struct Sub {
    char      pad[0x70];
    Consumer* f70;
};
struct Owner {
    void* f0;
    Sub*  f4;
    char  pad[0x14 - 8];
    int   f14;
    void  OnCreate();
    void  Base_OnCreate();
};

static int   g_base_called = 0;
static void* g_cons_self   = 0;
static int   g_arg_val     = 0;

void Owner::Base_OnCreate()
{
    g_base_called = 1;
}
void Consumer::Consume(int* arg)
{
    g_cons_self = this;
    g_arg_val   = *arg;
}

int main()
{
    Owner o;
    Sub s;
    Consumer c;
    o.f4 = &s;
    s.f70 = &c;
    o.f14 = 777;

    o.OnCreate();

    bool ok = g_base_called == 1 && g_cons_self == (void*)&c && g_arg_val == 777;
    if (ok) printf("PASS_UseReadable_OnCreate\n");
    else    printf("FAIL base=%d self=%p arg=%d\n", g_base_called, g_cons_self, g_arg_val);
    return ok ? 0 : 1;
}