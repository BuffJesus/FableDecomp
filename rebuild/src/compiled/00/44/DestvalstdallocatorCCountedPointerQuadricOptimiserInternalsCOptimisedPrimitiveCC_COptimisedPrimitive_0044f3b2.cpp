#pragma optimize("s",on)
struct Ctrl { int refcount; void (__fastcall *destroy)(void*); void* arg; };
struct CCP { void* val; Ctrl* ctrl; };

extern "C" void __cdecl op_delete(void*);

void __fastcall Dest_val(CCP* self)
{
    Ctrl* c = self->ctrl;
    if (c) {
        c->refcount--;
        c = self->ctrl;
        if (c->refcount == 0) {
            c->destroy(c->arg);
            op_delete(self->ctrl);
        }
    }
    self->val = 0;
    self->ctrl = 0;
}