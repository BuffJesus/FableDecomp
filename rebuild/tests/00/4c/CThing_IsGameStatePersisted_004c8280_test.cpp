#include <stdio.h>

struct CThing {
  char pad0[0x38];
  signed char f38;
  char pad1[0x92-0x39];
  unsigned char f92;
};

static bool model(CThing* self)
{
  signed char v = self->f38;
  if (v >= 0)
    return false;
  return (bool)((self->f92 >> 5) & 1);
}

int main()
{
  CThing t;
  t.f38 = 0x7f; t.f92 = 0xff;
  if (model(&t) != false) { printf("FAIL1\n"); return 1; }
  t.f38 = 0x00; t.f92 = 0x20;
  if (model(&t) != false) { printf("FAIL2\n"); return 1; }
  t.f38 = (signed char)0x80; t.f92 = 0x20;
  if (model(&t) != true) { printf("FAIL3\n"); return 1; }
  t.f38 = (signed char)0xff; t.f92 = 0x00;
  if (model(&t) != false) { printf("FAIL4\n"); return 1; }
  t.f38 = (signed char)0xff; t.f92 = 0xdf;
  if (model(&t) != false) { printf("FAIL5\n"); return 1; }
  t.f38 = (signed char)0x80; t.f92 = 0xff;
  if (model(&t) != true) { printf("FAIL6\n"); return 1; }
  printf("OK_004c8280\n");
  return 0;
}