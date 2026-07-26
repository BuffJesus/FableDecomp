struct Counted { void* vtbl; long refs; };
struct CIVCountedPointer { Counted* p; };

void __fastcall Dest_val(CIVCountedPointer* self)
{
    Counted* c = self->p;
    if (c) {
        if (--c->refs == 0) {
            (*(void (__fastcall**)(Counted*))((char*)c->vtbl + 4))(c);
        }
        self->p = 0;
    }
}