#include <cstdio>
struct Inner { char pad[0xA4]; float air; };
struct Outer { char pad[0xE0]; Inner* inner; };
static float GetAirResistance(Outer* self) { return self->inner->air; }
int main() {
 Inner iv; iv.air=(float)0.5f; Outer o; o.inner=&iv;
 if (GetAirResistance(&o) != (float)0.5f) { std::printf("006620e0_TEST FAIL\n"); return 1; }
 std::printf("006620e0_TEST PASS\n"); return 0;
}