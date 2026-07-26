struct RefCounted { void (**vtbl)(void); int refcount; };
struct CountedPtr { RefCounted* p; };

void __fastcall Dest_val(CountedPtr* self)
{
    RefCounted* obj = self->p;
    if (obj) {
        if (--obj->refcount == 0) {
            void (**vt)(void) = obj->vtbl;
            ((void (__fastcall*)(RefCounted*))vt[1])(obj);
        }
        self->p = 0;
    }
}