struct CDefClassBase;

struct CWifeDef {
    char pad[0x28];
    int f28;
    // base-class copy at 0x431f10; member => thiscall(this, src)
    void base_clone(const CDefClassBase* src);
};

void __fastcall CWifeDef_Copy(CWifeDef* self, int /*edx*/, const CDefClassBase* src)
{
    self->base_clone(src);
    self->f28 = ((const CWifeDef*)src)->f28;
}