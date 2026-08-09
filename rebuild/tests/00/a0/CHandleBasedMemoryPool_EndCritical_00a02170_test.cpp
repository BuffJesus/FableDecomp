#include <cstdio>
struct CRIT { int x; };
typedef void (*LEAVEFN)(void*);
static int g_called = 0;
static void* g_arg = 0;
void fake_leave(void* p){ g_called++; g_arg = p; }
LEAVEFN g_leave = fake_leave;

struct Pool {
    char pad[0x24];
    char flag;
    char pad2[0x44c - 0x25];
    CRIT cs;
};

void EndCritical(Pool* self){
    if (self->flag) { g_leave(&self->cs); }
}

int main(){
    Pool a; a.flag = 0; g_called = 0;
    EndCritical(&a);
    if (g_called != 0) { printf("FAIL flag0\n"); return 1; }

    Pool b; b.flag = 1; g_called = 0; g_arg = 0;
    EndCritical(&b);
    if (g_called != 1) { printf("FAIL flag1 not called\n"); return 1; }
    if (g_arg != (void*)&b.cs) { printf("FAIL wrong arg\n"); return 1; }

    printf("OK_00a02170\n");
    return 0;
}