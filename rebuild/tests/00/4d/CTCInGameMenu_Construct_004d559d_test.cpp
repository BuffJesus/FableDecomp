
#include <stdio.h>
#include <stdlib.h>
static int g_ctor=0;
struct Menu { void* parent; };
static Menu* ctor(Menu* self, void* parent){ self->parent=parent; ++g_ctor; return self; }
static Menu* Construct(void* parent){
    Menu* p = (Menu*)malloc(0xF8);
    if (p != 0) return ctor(p, parent);
    return 0;
}
int main(){
    void* par=(void*)0xABCD;
    Menu* m = Construct(par);
    int ok = (m!=0) && (m->parent==par) && (g_ctor==1);
    free(m);
    printf(ok ? "MENU_CONSTRUCT_OK\n" : "FAIL\n");
    return 0;
}