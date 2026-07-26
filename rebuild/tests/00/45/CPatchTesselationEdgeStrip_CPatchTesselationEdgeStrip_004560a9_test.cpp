#include <cstdio>

static int freed_count = 0;
static void* freed[8];
extern "C" void engine_free(void* p){ freed[freed_count++] = p; }

static int base_called = 0;

struct CPatchTesselationEdgeStrip {
  char pad[0x60];
  void* p60;
  char pad2[0x70 - 0x64];
  void* p70;
  char pad3[0x80 - 0x74];
  void* p80;
};

void __fastcall CPatchTesselationEdgeStripBase_dtor(void* self){ base_called = 1; }

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
  if (self->p80) engine_free(self->p80);
  if (self->p70) engine_free(self->p70);
  if (self->p60) engine_free(self->p60);
  CPatchTesselationEdgeStripBase_dtor(self);
}

int main(){
  CPatchTesselationEdgeStrip o;
  for (int i=0;i<(int)sizeof(o);++i) ((char*)&o)[i]=0;
  o.p80 = (void*)0x1000;
  o.p70 = (void*)0x2000;
  o.p60 = 0; /* skip this one */
  CPatchTesselationEdgeStrip_dtor(&o);
  if (freed_count == 2 && freed[0]==(void*)0x1000 && freed[1]==(void*)0x2000 && base_called==1) {
    std::printf("CPatchTesselationEdgeStrip_004560a9_TEST PASS\n");
    return 0;
  }
  std::printf("FAIL count=%d base=%d\n", freed_count, base_called);
  return 1;
}