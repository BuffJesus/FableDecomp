#include <cstdio>
struct S { char pad[0x148]; long* p148; long GetAbilityLevel(long i); };
long S::GetAbilityLevel(long i){ return this->p148[i]; }
int main(){ long arr[4]={0,0,77,0}; S o; o.p148=arr; if(o.GetAbilityLevel(2)!=77){ std::printf("005ee136_TEST FAIL\n"); return 1;}
 std::printf("005ee136_TEST PASS\n"); return 0;}