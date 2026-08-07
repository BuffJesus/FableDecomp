#include <cstdio>
struct E { char pad[0x1c]; int v1c; char pad2[0x48-0x20]; };
struct S { char pad[0x20]; E* f20; int* GetLevelScriptName(int i); };
int* S::GetLevelScriptName(int i){ return &this->f20[i].v1c; }
int main(){ E arr[3]; S o; o.f20=arr;
 if(o.GetLevelScriptName(2)!=&arr[2].v1c){ std::printf("004fcfc0_TEST FAIL\n"); return 1;}
 std::printf("004fcfc0_TEST PASS\n"); return 0;}