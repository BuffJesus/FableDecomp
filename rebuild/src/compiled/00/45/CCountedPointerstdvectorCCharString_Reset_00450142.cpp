#pragma optimize("s",on)
#include <new>
struct RefBlock { int count; int vtbl; void* ptr; };
struct CCountedPtr { void* obj; RefBlock* blk; };
extern "C" void __fastcall CCountedPtr_helper(CCountedPtr* self);
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