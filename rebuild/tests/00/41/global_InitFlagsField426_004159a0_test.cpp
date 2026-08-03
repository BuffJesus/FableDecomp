#include <cstdio>

struct T {
  char pad0[4];
  unsigned char f4;
  char pad5[0x26-5];
  unsigned char f26;
  T* InitFlags_Field4_26();
};

T* T::InitFlags_Field4_26() {
  this->f4 = 0xff;
  this->f26 = 0;
  return this;
}

int main() {
  T obj;
  obj.f4 = 0x11;
  obj.f26 = 0x22;
  T* r = obj.InitFlags_Field4_26();
  if (r != &obj) { printf("FAIL ret\n"); return 1; }
  if (obj.f4 != 0xff) { printf("FAIL f4=%02x\n", obj.f4); return 1; }
  if (obj.f26 != 0x00) { printf("FAIL f26=%02x\n", obj.f26); return 1; }
  printf("OK_0x004159a0\n");
  return 0;
}