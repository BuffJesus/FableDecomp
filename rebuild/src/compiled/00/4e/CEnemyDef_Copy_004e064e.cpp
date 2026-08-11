struct CDefClassBase;

struct CEnemyDef {
    char pad[0x28];
    int f28;
    // base-class copy; member => thiscall(this, src)
    void base_clone(const CDefClassBase* src);
};

void __fastcall CEnemyDef_Copy(CEnemyDef* self, int /*edx*/, const CDefClassBase* src)
{
    self->base_clone(src);
    self->f28 = ((const CEnemyDef*)src)->f28;
}