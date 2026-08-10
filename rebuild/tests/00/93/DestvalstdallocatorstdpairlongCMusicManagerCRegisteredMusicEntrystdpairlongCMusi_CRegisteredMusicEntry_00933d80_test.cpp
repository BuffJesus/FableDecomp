#include <cstdio>
struct Sub { int v; };
struct Obj { char pad[0xa8]; Sub sub; int base; };
static int g_sub = 0;
static int g_base = 0;
void SubDtor(Sub* s){ g_sub = s->v + 1; }
void BaseDtor(Obj* o){ g_base = o->base + 1; }
void Model(Obj* self){ SubDtor(&self->sub); BaseDtor(self); }
int main(){
    Obj o; o.sub.v = 41; o.base = 10;
    Model(&o);
    if (g_sub != 42) { printf("FAIL sub\n"); return 1; }
    if (g_base != 11) { printf("FAIL base\n"); return 1; }
    printf("OK_00933d80\n");
    return 0;
}