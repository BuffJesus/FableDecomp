struct CDefClassBase;

struct CVillageMemberBase {
  void Duplicate(const CDefClassBase* src);  // 0x431f10, __fastcall
};

struct CVillageMemberDef {
  char pad[0x25];
  unsigned char f25;
};

void __fastcall CVillageMemberDef_Copy(CVillageMemberDef* self, void* edx, const CDefClassBase* src)
{
  reinterpret_cast<CVillageMemberBase*>(self)->Duplicate(src);
  self->f25 = reinterpret_cast<const CVillageMemberDef*>(src)->f25;
}