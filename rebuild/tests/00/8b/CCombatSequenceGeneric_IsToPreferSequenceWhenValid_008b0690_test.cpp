#include <cstdio>
struct T { char pad[0x48]; int f48; char pad2[0x64-0x4c]; unsigned char b64; };
struct S { char pad[0x28]; T* f28; char M(); };
char S::M(){ T* p=this->f28; if(p->f48) return (char)p->b64; return 0; }
int main(){ T t; S o; o.f28=&t; t.f48=1; t.b64=9;
 if(o.M()!=9){ std::printf("008b0690_TEST FAIL\n"); return 1;}
 t.f48=0; if(o.M()!=0){ std::printf("008b0690_TEST FAIL\n"); return 1;}
 std::printf("008b0690_TEST PASS\n"); return 0;}