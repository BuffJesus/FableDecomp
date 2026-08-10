#include <cstdio>
struct Sub { int x; };
struct T { char pad[0xa8]; Sub sub; };

static int g_sub_called = 0;
static int g_finish_called = 0;
static Sub* g_sub_arg = 0;
static T* g_finish_arg = 0;

void Sub_release(Sub* s) { g_sub_called++; g_sub_arg = s; }
void T_finish(T* self) { g_finish_called++; g_finish_arg = self; }

void DestVal_A8_I(T* self)
{
    Sub_release(&self->sub);
    T_finish(self);
}

int main()
{
    T obj;
    DestVal_A8_I(&obj);
    if (g_sub_called != 1) { printf("FAIL sub_called=%d\n", g_sub_called); return 1; }
    if (g_finish_called != 1) { printf("FAIL finish_called=%d\n", g_finish_called); return 1; }
    if (g_sub_arg != &obj.sub) { printf("FAIL sub_arg\n"); return 1; }
    if (g_finish_arg != &obj) { printf("FAIL finish_arg\n"); return 1; }
    if ((char*)&obj.sub - (char*)&obj != 0xa8) { printf("FAIL offset\n"); return 1; }
    printf("OK_0095e080\n");
    return 0;
}