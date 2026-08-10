struct Sub114 { void teardown(); };
struct Owner { char pad[0x114]; Sub114 sub; };
extern "C" void __fastcall NextTeardown(Owner* self);

struct Host {
  char pad[0x114];
  Sub114 sub;
  void DestVal_114_B();
};

void __fastcall DestVal_114_B(Host* self) {
  self->sub.teardown();
  NextTeardown((Owner*)self);
}