struct RefTarget { void (__fastcall **vtbl)(void*); int refcount; };
struct CountedPtr { RefTarget* p; };

void __fastcall Dest_val(CountedPtr* self)
{
    RefTarget* obj = self->p;
    if (obj != 0) {
        if (--obj->refcount == 0) {
            obj->vtbl[1](obj);
        }
        self->p = 0;
    }
}