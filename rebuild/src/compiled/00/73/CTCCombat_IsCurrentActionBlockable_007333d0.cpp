struct CInner { char pad[0x100]; void* p100; };
struct CVtbl { char pad48[0x48]; unsigned char (__fastcall *m)(void*); };
struct CTCCombat { int f0,f4,f8,fC; char pad[0x24-0x10]; CInner* f24; };

static bool checkframe(int v)
{
  return v == 0 || v == (int)0xfffffcf7;
}

bool __fastcall CTCCombat_IsCurrentActionBlockable(CTCCombat* self)
{
  void* obj = self->f24->p100;
  if (obj != 0)
  {
    CVtbl** vt = (CVtbl**)obj;
    if (!(**vt).m(obj))
      return checkframe(self->fC);
  }
  return false;
}