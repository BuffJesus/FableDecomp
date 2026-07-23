#include <cstdio>

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

static long __fastcall stub_slot06(CCreatureActionBase*) { return 100; }

int main() {
  CCAB_vtbl vt;
  vt.slot00 = vt.slot01 = vt.slot02 = 0;
  vt.slot03 = vt.slot04 = vt.slot05 = 0;
  vt.slot06 = stub_slot06;
  CCreatureActionBase obj;
  obj.vtbl = &vt;
  obj.frameOffset = 23;
  long r = CCreatureActionBase_GetAnimStartFrame(&obj);
  if (r == 123) { std::printf("CCreatureActionBase_00692470_TEST PASS\n"); return 0; }
  std::printf("FAIL got %ld\n", r); return 1;
}