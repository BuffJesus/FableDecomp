#include <cstdio>
struct S { char pad[0x18]; int* arr; };
static int GetAbilityLevel(S* self, int index){ return self->arr[index]; }
int main(){ int vals[4]={10,20,30,40}; S obj; obj.arr=vals;
 if(GetAbilityLevel(&obj,2)!=30){ std::printf("006d6510_TEST FAIL\n"); return 1;}
 std::printf("006d6510_TEST PASS\n"); return 0;}