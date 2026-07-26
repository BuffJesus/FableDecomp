#pragma optimize("s",on)
extern "C" void engine_free(void* p);

struct CPatchTesselationEdgeStripBase {
};

extern void __fastcall CPatchTesselationEdgeStripBase_dtor(void* self);

struct CPatchTesselationEdgeStrip {
  char pad[0x60];
  void* p60;   /* +0x60 */
  char pad2[0x70 - 0x64];
  void* p70;   /* +0x70 */
  char pad3[0x80 - 0x74];
  void* p80;   /* +0x80 */
};

void __fastcall CPatchTesselationEdgeStrip_dtor(CPatchTesselationEdgeStrip* self)
{
  if (self->p80) engine_free(self->p80);
  if (self->p70) engine_free(self->p70);
  if (self->p60) engine_free(self->p60);
  CPatchTesselationEdgeStripBase_dtor(self);
}