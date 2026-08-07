#include <cstdio>
struct E { char pad[0x20]; int v20; char pad2[0x48-0x24]; };
struct S { char pad[0x20]; E* f20; int GetEditMap(int i); };
int S::GetEditMap(int i){ return this->f20[i].v20; }
int main(){ E arr[3]; arr[2].v20=42; S o; o.f20=arr;
 if(o.GetEditMap(2)!=42){ std::printf("004fce00_TEST FAIL\n"); return 1;}
 std::printf("004fce00_TEST PASS\n"); return 0;}