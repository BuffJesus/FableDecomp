struct CDefClassBase;

struct CThingSwitchDef {
    char pad[0x70];
    int f70;
    // base-class copy at 0x463950; member => thiscall(this, src)
    void base_clone(const CDefClassBase* src);
};

void __fastcall CThingSwitchDef_Copy(CThingSwitchDef* self, int /*edx*/, const CDefClassBase* src)
{
    self->base_clone(src);
    self->f70 = ((const CThingSwitchDef*)src)->f70;
}