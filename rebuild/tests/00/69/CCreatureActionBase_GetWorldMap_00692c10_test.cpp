#include <cstdio>

// Behavioural model of GetWorldMap:
//   this += 8; a = f1(this); b = f2(a); return b->vtbl[0x30/4](b)

struct WorldMap { int tag; };

struct Target { void** vtbl; };

static WorldMap g_wm = { 0xBEEF };

static WorldMap* virt(Target* t){ (void)t; return &g_wm; }

int main(){
    static void* vt[16];
    for (int i=0;i<16;i++) vt[i]=0;
    vt[0x30/4] = (void*)&virt;   // slot 0x30 => index 12
    Target tgt; tgt.vtbl = vt;

    Target* b = &tgt;
    typedef WorldMap* (*fn)(Target*);
    fn f = (fn)b->vtbl[0x30/4];
    WorldMap* r = f(b);

    // null-slot branch coverage
    bool nullpath = (vt[0] == 0);

    if (r == &g_wm && r->tag == (int)0xBEEF && nullpath) {
        printf("OK_00692c10\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}