struct CTCInventoryExperience {
  char pad[0x1c];
  int field_1c;
};
extern "C" void __fastcall Helper_00576c0b(CTCInventoryExperience* self);
extern "C" void __fastcall Tail_005bc66f(CTCInventoryExperience* self);
extern "C" void __fastcall ProcessButtonBReleased(CTCInventoryExperience* self)
{
  if (self->field_1c == 1) {
    Helper_00576c0b(self);
  }
  Tail_005bc66f(self);
}