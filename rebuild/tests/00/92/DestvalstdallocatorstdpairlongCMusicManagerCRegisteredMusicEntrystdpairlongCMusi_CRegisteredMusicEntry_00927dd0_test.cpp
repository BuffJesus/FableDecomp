
#include <cstdio>
struct Sub { int v; };
struct Obj { char pad[0xa8]; Sub sub_a8; int tag; };

static int g_helper_calls=0;
static int g_tail_calls=0;
static Sub* g_last_sub=0;
static Obj* g_last_self=0;

void helper(Sub* s){ g_helper_calls++; g_last_sub=s; s->v=1; }
void tailf(Obj* self){ g_tail_calls++; g_last_self=self; self->tag=42; }

void model(Obj* self){ helper(&self->sub_a8); tailf(self); }

int main(){
    Obj o; o.tag=0; o.sub_a8.v=0;
    model(&o);
    int ok=1;
    if(g_helper_calls!=1) ok=0;
    if(g_tail_calls!=1) ok=0;
    if(g_last_sub!=&o.sub_a8) ok=0;
    if(g_last_self!=&o) ok=0;
    if(o.tag!=42) ok=0;
    if(o.sub_a8.v!=1) ok=0;
    o.tag=5; o.sub_a8.v=9;
    model(&o);
    if(o.tag!=42||o.sub_a8.v!=1) ok=0;
    if(ok) printf("OK_00927dd0\n"); else printf("FAIL\n");
    return ok?0:1;
}