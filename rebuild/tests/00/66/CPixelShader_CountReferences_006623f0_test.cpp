#include <cstdio>
struct Inner { char pad[0xD8]; int refs; };
struct Outer { char pad[0xE0]; Inner* inner; };
static int CountReferences(Outer* self) { return self->inner->refs; }
int main() {
 Inner iv; iv.refs=(int)1234; Outer o; o.inner=&iv;
 if (CountReferences(&o) != (int)1234) { std::printf("006623f0_TEST FAIL\n"); return 1; }
 std::printf("006623f0_TEST PASS\n"); return 0;
}