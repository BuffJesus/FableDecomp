#include <cstdio>
struct S { char pad[0x2c]; long fv; void AddExperience(long a); };
void S::AddExperience(long a){ this->fv += a; }
int main(){ S o; o.fv=100; o.AddExperience(23); if(o.fv!=123){ std::printf("009fc160_TEST FAIL\n"); return 1;}
 std::printf("009fc160_TEST PASS\n"); return 0;}