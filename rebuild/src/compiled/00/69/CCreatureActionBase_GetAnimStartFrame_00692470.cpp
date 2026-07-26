struct CCreatureActionBase;
typedef long (__fastcall *pfn_t)(CCreatureActionBase*);
struct CCAB_vtbl {
  pfn_t slot00; pfn_t slot01; pfn_t slot02;
  pfn_t slot03; pfn_t slot04; pfn_t slot05;
  pfn_t slot06;
};
struct CCreatureActionBase {
  CCAB_vtbl* vtbl;
  char pad[0x44];
  long frameOffset;
};

long __fastcall CCreatureActionBase_GetAnimStartFrame(CCreatureActionBase* self)
{
  return self->vtbl->slot06(self) + self->frameOffset;
}