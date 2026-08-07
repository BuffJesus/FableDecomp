#include <cstdio>
struct Base { char pad[0x74]; float* arr; };
struct S { char pad0[0x0C]; Base* base; char pad1[0x0C]; int idx; };
static float GetDamageMultiplier(S* self){ return self->base->arr[self->idx]; }
int main(){ float vals[3]={1.f,2.5f,3.f}; Base bse; bse.arr=vals; S obj; obj.base=&bse; obj.idx=1;
 if(GetDamageMultiplier(&obj)!=2.5f){ std::printf("007cbf50_TEST FAIL\n"); return 1;}
 std::printf("007cbf50_TEST PASS\n"); return 0;}