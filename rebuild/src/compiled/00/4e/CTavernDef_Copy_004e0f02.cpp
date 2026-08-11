struct CDefClassBase;

struct CTavernDef {
    char pad[0x28];
    int f28;
    // base-class copy; member => thiscall(this, src)
    void base_clone(const CDefClassBase* src);
};

void __fastcall CTavernDef_Copy(CTavernDef* self, int /*edx*/, const CDefClassBase* src)
{
    self->base_clone(src);
    self->f28 = ((const CTavernDef*)src)->f28;
}