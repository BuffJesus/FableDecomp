#include <cstdio>
struct T { char pad[0x148]; unsigned char b148; };
struct S { char pad[0xe0]; T* fe0; char M(); };
char S::M(){ unsigned char v=this->fe0->b148; v>>=1; v&=1; return (char)v; }
int main(){ T t; S o; o.fe0=&t;
 t.b148=2; if(o.M()!=1){ std::printf("006621a0_TEST FAIL\n"); return 1;}
 t.b148=0; if(o.M()!=0){ std::printf("006621a0_TEST FAIL\n"); return 1;}
 std::printf("006621a0_TEST PASS\n"); return 0;}