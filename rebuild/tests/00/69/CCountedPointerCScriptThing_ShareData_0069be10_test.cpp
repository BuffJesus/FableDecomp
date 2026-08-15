#include <cstdio>
struct Ctl { long rc; void (*fn)(void*); void* ctx; };
struct CCPtr { void* info; Ctl* p; };
static int g_rel=0, g_free=0;
static void relfn(void* c){ g_rel++; }
static void doFree(void* p){ g_free++; }
static void ShareData(CCPtr* self, void* newInfo, Ctl* np){
    if(self->p != np){
        if(self->p){
            if(--self->p->rc==0){ self->p->fn(self->p->ctx); doFree(self->p); }
        }
        self->info=newInfo; self->p=np; if(np) ++np->rc;
    }
}
int main(){
    Ctl a; a.rc=1; a.fn=relfn; a.ctx=0;
    Ctl b; b.rc=1; b.fn=relfn; b.ctx=0;
    CCPtr sp; sp.info=0; sp.p=&a;
    ShareData(&sp,(void*)0x1234,&b);
    ShareData(&sp,(void*)0x1,&b);          // p==np -> no-op
    if(sp.p==&b && b.rc==2 && a.rc==0 && g_rel==1 && g_free==1 && sp.info==(void*)0x1234)
        printf("PARITY_OK\n");
    else printf("FAIL r=%d f=%d arc=%ld brc=%ld info=%p\n",g_rel,g_free,a.rc,b.rc,sp.info);
    return 0;
}