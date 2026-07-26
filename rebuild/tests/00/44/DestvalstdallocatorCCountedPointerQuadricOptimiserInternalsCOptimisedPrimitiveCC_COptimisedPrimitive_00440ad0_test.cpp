#include <cstdio>

static int g_freed = 0;
static int g_dtored = 0;
extern "C" void __cdecl fable_free(void* p){ (void)p; g_freed++; }

struct Counter {
    long refcount;
    void (__fastcall *dtor)(void*);
    void* obj;
};

struct CCP {
    void* p0;
    Counter* pctr;
};

static void __fastcall obj_dtor(void* o){ (void)o; g_dtored++; }

void __fastcall _Dest_val(CCP* self)
{
    Counter* c = self->pctr;
    if (c) {
        (self->pctr->refcount)--;
        c = self->pctr;
        if (c->refcount == 0) {
            (c->dtor)(c->obj);
            fable_free(self->pctr);
        }
    }
    self->p0 = 0;
    self->pctr = 0;
}

int main(){
    // Case 1: refcount goes to zero -> dtor + free
    Counter ctr; ctr.refcount = 1; ctr.dtor = obj_dtor; ctr.obj = (void*)0x1234;
    CCP a; a.p0 = (void*)0xAAAA; a.pctr = &ctr;
    _Dest_val(&a);
    if (a.p0 != 0 || a.pctr != 0) { std::printf("FAIL clear\n"); return 1; }
    if (g_dtored != 1 || g_freed != 1) { std::printf("FAIL destroy\n"); return 1; }

    // Case 2: refcount > 1 -> no dtor/free, just decrement + clear
    Counter ctr2; ctr2.refcount = 3; ctr2.dtor = obj_dtor; ctr2.obj = 0;
    CCP b; b.p0 = (void*)0xBBBB; b.pctr = &ctr2;
    _Dest_val(&b);
    if (b.p0 != 0 || b.pctr != 0) { std::printf("FAIL clear2\n"); return 1; }
    if (ctr2.refcount != 2) { std::printf("FAIL dec\n"); return 1; }
    if (g_dtored != 1 || g_freed != 1) { std::printf("FAIL nodtor\n"); return 1; }

    // Case 3: null pointer -> just clear
    CCP d; d.p0 = (void*)0xDDDD; d.pctr = 0;
    _Dest_val(&d);
    if (d.p0 != 0 || d.pctr != 0) { std::printf("FAIL clear3\n"); return 1; }

    std::printf("destval_00440ad0_TEST PASS\n");
    return 0;
}