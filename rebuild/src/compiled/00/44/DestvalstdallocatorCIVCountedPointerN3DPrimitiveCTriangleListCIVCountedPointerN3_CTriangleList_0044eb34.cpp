#pragma optimize("s",on)
struct CountedObj { void (__fastcall **vtbl)(CountedObj*); long refcount; };
struct CIVCountedPointer { CountedObj* obj; };

void __fastcall Dest_val(CIVCountedPointer* self, void* alloc)
{
    CountedObj* o = self->obj;
    if (o) {
        if (--o->refcount == 0) {
            o->vtbl[1](o);
        }
        self->obj = 0;
    }
}