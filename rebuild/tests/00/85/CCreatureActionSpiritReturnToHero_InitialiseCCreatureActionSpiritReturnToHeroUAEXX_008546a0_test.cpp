#include <cstdio>
struct S { char pad[0x10]; long f10; int f14; };
int g_val = 42;
int base_called = 0;
void base_init(S* s){ base_called = 1; }
void Initialise(S* s){ base_init(s); s->f14 = 0; s->f10 = (long)(float)g_val; }
int main(){
    S s; s.f10 = -1; s.f14 = -1;
    Initialise(&s);
    if(base_called==1 && s.f14==0 && s.f10==42) printf("PARITY_OK\n");
    else printf("FAIL %d %d %ld\n", base_called, s.f14, s.f10);
    return 0;
}