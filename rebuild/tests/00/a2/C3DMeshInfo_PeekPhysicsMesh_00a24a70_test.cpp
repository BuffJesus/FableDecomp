#include <cstdio>
struct A { char pad[0x380]; void** arr; };
struct S { char pad[4]; A* f4; char pad2[0x3c-8]; int idx; };
void* __fastcall PeekPhysicsMesh(S* self){ A* a=self->f4; return a->arr[self->idx]; }
int main(){ void* arr[4]={0,0,(void*)7,0}; A a; a.arr=arr; S o; o.f4=&a; o.idx=2;
 if(PeekPhysicsMesh(&o)!=(void*)7){ std::printf("00a24a70_TEST FAIL\n"); return 1;}
 std::printf("00a24a70_TEST PASS\n"); return 0;}