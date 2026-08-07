#include <cstdio>
struct Sub { int Method(int x){ return x + 1; } };
struct C { char pad[4]; Sub* f4; int f8; int GetLength(){ return this->f4->Method(this->f8); } };
int main(){ Sub s; C o; o.f4 = &s; o.f8 = 41;
 if (o.GetLength() != 42) { std::printf("00a39c20_TEST FAIL\n"); return 1; }
 std::printf("00a39c20_TEST PASS\n"); return 0; }