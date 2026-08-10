struct CTCOnHeroPush {
  char pad[0x3c];
  float f3c;
  float f40;
};
bool __fastcall IsAngry(CTCOnHeroPush* self) {
  return self->f3c >= self->f40;
}