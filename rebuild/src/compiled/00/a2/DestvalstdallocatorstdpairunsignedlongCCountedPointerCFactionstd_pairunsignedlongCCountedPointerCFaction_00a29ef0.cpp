extern "C" void __cdecl fable_op_delete(void* p);

struct CtrlBlock {
    long refcount;      /* +0x0 */
    void (__fastcall* dtor)(void* self); /* +0x4 */
    void* obj;          /* +0x8 */
};

struct Pair {
    unsigned long first; /* +0x0 */
    void* dummy;         /* +0x4 */
    CtrlBlock* ctrl;     /* +0x8 : CCountedPointer control block */
};

void __fastcall Dest_val(Pair* self)
{
    CtrlBlock* cb = self->ctrl;
    if (cb != 0) {
        --cb->refcount;
        cb = self->ctrl;
        if (cb->refcount == 0) {
            cb->dtor(cb->obj);
            fable_op_delete(self->ctrl);
        }
    }
    self->dummy = 0;
    self->ctrl = 0;
}