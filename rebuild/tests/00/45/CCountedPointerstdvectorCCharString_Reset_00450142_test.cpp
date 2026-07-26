#include <cstdio>
#include <new>
struct RefBlock { int count; int vtbl; void* ptr; };
struct CCountedPtr { void* obj; RefBlock* blk; };
static int g_helper_called = 0;
extern "C" void __fastcall CCountedPtr_helper(CCountedPtr* self){ g_helper_called++; }
void __fastcall CCountedPtr_Reset(CCountedPtr* self, int edxdummy, void* arg)
{
    CCountedPtr_helper(self);
    void* a = arg;
    self->obj = a;
    if (a) {
        RefBlock* b = (RefBlock*)::operator new(0xc);
        RefBlock* r;
        if (b) {
            void* o = self->obj;
            b->count = 1;
            b->vtbl = 0x44eafd;
            b->ptr = o;
            r = b;
        } else { r = 0; }
        self->blk = r;
    }
}
int main(){
    CCountedPtr cp; cp.obj=(void*)0xdead; cp.blk=(RefBlock*)0xbeef;
    int dummy=0x1234;
    CCountedPtr_Reset(&cp, 0, &dummy);
    if(g_helper_called!=1){ std::printf("FAIL helper\n"); return 1; }
    if(cp.obj!=&dummy){ std::printf("FAIL obj\n"); return 1; }
    RefBlock* b=cp.blk;
    if(b==0){ std::printf("FAIL blk null\n"); return 1; }
    if(b->count!=1 || b->vtbl!=0x44eafd || b->ptr!=&dummy){ std::printf("FAIL fields\n"); return 1; }
    ::operator delete(b);
    CCountedPtr cp2; cp2.obj=(void*)0x1; cp2.blk=(RefBlock*)0x99;
    CCountedPtr_Reset(&cp2, 0, 0);
    if(cp2.obj!=0){ std::printf("FAIL obj2\n"); return 1; }
    if(cp2.blk!=(RefBlock*)0x99){ std::printf("FAIL blk2 unchanged\n"); return 1; }
    std::printf("CCountedPointer_00450142_TEST PASS\n");
    return 0;
}