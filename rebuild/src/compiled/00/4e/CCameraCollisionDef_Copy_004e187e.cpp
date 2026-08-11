// CCameraCollisionDef::Copy(CDefClassBase const*)
struct CDefClassBase;

struct CCameraCollisionDef {
    char pad[0x28];
    unsigned long field28;  // +0x28
};

// Base-class Copy, called via rel32.
extern void __fastcall CDefClassBase_Copy(void* self, void* edx, const CDefClassBase* src);

void __fastcall CCameraCollisionDef_Copy(CCameraCollisionDef* self, void* edx, const CDefClassBase* src)
{
    CDefClassBase_Copy(self, edx, src);
    self->field28 = ((const CCameraCollisionDef*)src)->field28;
}