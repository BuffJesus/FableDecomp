#include <cstdio>
struct E { char pad[0x27]; char v27; char pad2[0x48-0x28]; };
struct S { char pad[0x20]; E* f20; char IsMapLoadedOnPlayerProximity(int i); };
char S::IsMapLoadedOnPlayerProximity(int i){ return this->f20[i].v27; }
int main(){ E arr[3]; arr[2].v27=5; S o; o.f20=arr;
 if(o.IsMapLoadedOnPlayerProximity(2)!=5){ std::printf("004fd000_TEST FAIL\n"); return 1;}
 std::printf("004fd000_TEST PASS\n"); return 0;}