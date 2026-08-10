#include <cstdio>
struct CTCOnHeroPush {
  char pad[0x3c];
  float f3c;
  float f40;
};
static bool IsAngry_model(CTCOnHeroPush* self) {
  return self->f3c >= self->f40;
}
int main() {
  CTCOnHeroPush a; a.f3c = 5.0f; a.f40 = 3.0f;
  CTCOnHeroPush b; b.f3c = 1.0f; b.f40 = 2.0f;
  CTCOnHeroPush c; c.f3c = 4.0f; c.f40 = 4.0f;
  if (!IsAngry_model(&a)) { printf("FAIL a\n"); return 1; }
  if (IsAngry_model(&b)) { printf("FAIL b\n"); return 1; }
  if (!IsAngry_model(&c)) { printf("FAIL c\n"); return 1; }
  printf("OK_00c25de0\n");
  return 0;
}