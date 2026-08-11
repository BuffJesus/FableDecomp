#include <cstdio>

struct CDefClassBase;

struct CVillageMemberBase {
  void Duplicate(const CDefClassBase* src);
};

struct CVillageMemberDef {
  char pad[0x25];
  unsigned char f25;
};

static int g_base_called = 0;
void CVillageMemberBase::Duplicate(const CDefClassBase* src) {
  g_base_called = 1;
}

void __fastcall CVillageMemberDef_Copy(CVillageMemberDef* self, void* edx, const CDefClassBase* src);

int main() {
  CVillageMemberDef dst; dst.f25 = 0;
  CVillageMemberDef src; src.f25 = 0xAB;
  CVillageMemberDef_Copy(&dst, 0, reinterpret_cast<const CDefClassBase*>(&src));
  if (g_base_called == 1 && dst.f25 == 0xAB) {
    printf("004dffa5_TEST PASS\n");
  } else {
    printf("FAIL base=%d f25=%02x\n", g_base_called, dst.f25);
  }
  return 0;
}