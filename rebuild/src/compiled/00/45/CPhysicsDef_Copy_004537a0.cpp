struct CPhysicsDef;
extern "C" void __fastcall CPhysicsDef_CopyImpl(CPhysicsDef* self, void* edx, const CPhysicsDef* other);

void __fastcall CPhysicsDef_Copy(CPhysicsDef* self, void* edx, const CPhysicsDef* other)
{
    CPhysicsDef_CopyImpl(self, edx, other);
}