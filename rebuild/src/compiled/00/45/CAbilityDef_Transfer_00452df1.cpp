struct CTransferData { void Accept(void* p); };
struct CAbilityDef { char pad[0x28]; };

void __fastcall CAbilityDef_Transfer(CAbilityDef* self, void* edx, CTransferData* t)
{
    t->Accept((char*)self + 0x28);
}