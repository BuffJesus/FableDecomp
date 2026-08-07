#include <cstdio>
struct S { char pad[0xc]; long fv; void MovePosition(long a); };
void S::MovePosition(long a){ this->fv += a; }
int main(){ S o; o.fv=100; o.MovePosition(23); if(o.fv!=123){ std::printf("00a39c00_TEST FAIL\n"); return 1;}
 std::printf("00a39c00_TEST PASS\n"); return 0;}