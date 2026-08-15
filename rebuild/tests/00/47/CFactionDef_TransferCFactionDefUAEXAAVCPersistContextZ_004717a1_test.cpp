#include <cstdio>
struct RecCtx { void* seen[8]; int n; RecCtx():n(0){} void helper(void* p){seen[n++]=p;} };
struct SubA { int x; };
struct SubB { int y; };
struct CFactionDef { char pad[0x3c]; SubA a; char pad2[0x48-0x3c-sizeof(SubA)]; SubB b; };
static void Transfer(CFactionDef* self, RecCtx* ctx){ ctx->helper(&self->a); ctx->helper(&self->b); }
int main(){ CFactionDef def; RecCtx ctx; Transfer(&def,&ctx); char* base=(char*)&def;
    bool ok = ctx.n==2 && ctx.seen[0]==(void*)(base+0x3c) && ctx.seen[1]==(void*)(base+0x48);
    if(ok) printf("TRANSFER_OK\n"); else printf("FAIL n=%d\n",ctx.n); return 0; }