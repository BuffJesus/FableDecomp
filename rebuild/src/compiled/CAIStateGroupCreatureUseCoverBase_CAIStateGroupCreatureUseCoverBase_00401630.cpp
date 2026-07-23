struct Sub;
extern void __fastcall sub_5992f0(void* self);
extern void __fastcall sub_599ee0(void* member);
extern void __fastcall base_dtor(void* self);

struct CAIStateGroup_CreatureUseCoverBase {
    unsigned int vtbl;
    char pad[0x14];
    char member18[4];
};

void __fastcall CAIStateGroup_CreatureUseCoverBase_dtor(CAIStateGroup_CreatureUseCoverBase* self)
{
    self->vtbl = 0x122d06c;
    sub_5992f0(self);
    sub_599ee0(&self->member18);
    base_dtor(self);
}