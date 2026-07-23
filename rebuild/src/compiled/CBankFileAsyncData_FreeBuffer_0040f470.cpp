extern void __cdecl EngineFree(void* p);

struct CBankFileAsyncData {
  char pad0[0xc];
  void* p0c;      /* +0x0c */
  char pad10[0x1c-0x10];
  unsigned int v1c; /* +0x1c */
  void* p20;      /* +0x20 */
  void Inner(void* a, unsigned int b);
};

void __fastcall CBankFileAsyncData_FreeBuffer(CBankFileAsyncData* self)
{
  if (self->p20 != 0) {
    self->Inner(self->p0c, self->v1c + 4);
    if (self->p20 != 0) {
      EngineFree(self->p20);
    }
  }
}