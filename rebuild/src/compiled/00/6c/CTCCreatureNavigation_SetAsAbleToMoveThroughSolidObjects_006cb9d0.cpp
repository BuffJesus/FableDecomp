struct CObj;
struct CObjVtbl {
    void (__fastcall *slots[32])(CObj*);
};
struct CObj {
    CObjVtbl* vtbl;
};
struct CTCCreatureNavigation {
    char pad0[0x0c];
    CObj* obj;
    char pad1[0x55 - 0x10];
    unsigned char flag;
};

void __fastcall CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects(CTCCreatureNavigation* self, void* edx, unsigned char b)
{
    if (b != self->flag) {
        CObj* o = self->obj;
        o->vtbl->slots[0x48/4](o);
    }
    self->flag = b;
}