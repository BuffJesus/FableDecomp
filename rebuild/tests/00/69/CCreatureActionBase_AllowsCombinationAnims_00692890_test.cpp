#include <cstdio>
struct S;
struct S { char pad[0x74]; void* f74; char M(); };
int __fastcall sub(void* p){ return (int)(long)p; }
char S::M(){ unsigned int r=(unsigned int)sub(this->f74); return (char)((~(r>>3))&1); }
int main(){ S o; o.f74=(void*)8; if(o.M()!=0){ std::printf("00692890_TEST FAIL\n"); return 1;}
 o.f74=(void*)0; if(o.M()!=1){ std::printf("00692890_TEST FAIL\n"); return 1;}
 std::printf("00692890_TEST PASS\n"); return 0;}