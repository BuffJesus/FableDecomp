#include <cstdio>
// Independent behaviour model of _Cons_val: construct both pair members only when self != 0.
struct CCharString { int p; };
struct CArrayUL { int p; };
struct Pair { CCharString first; CArrayUL second; };

static int g_first = 0, g_second = 0;
static Pair* g_first_target = 0;
static Pair* g_second_target = 0;

struct Alloc {
    void ctorFirst(CCharString* d){ g_first++; g_first_target = (Pair*)d; }
    void ctorSecond(CArrayUL* d){ g_second++; g_second_target = (Pair*)((char*)d - 4); }
};

void Cons_val(Alloc* self, Pair* dst){
    if (self) {
        self->ctorFirst(&dst->first);
        self->ctorSecond(&dst->second);
    }
}

int main(){
    Pair p; Alloc a;
    Cons_val(0, &p);
    if (g_first != 0 || g_second != 0) { printf("FAIL null\n"); return 1; }
    Cons_val(&a, &p);
    if (g_first != 1 || g_second != 1) { printf("FAIL count\n"); return 1; }
    if (g_first_target != &p || g_second_target != &p) { printf("FAIL target\n"); return 1; }
    printf("CONS_VAL_OK\n");
    return 0;
}