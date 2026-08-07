#include <cstdio>
struct S { char* M(int i); };
char* S::M(int i){ return (char*)this + i*0x648 + 4; }
int main(){ S o; if(o.M(2)!=(char*)&o + 2*0x648 + 4){ std::printf("009f1730_TEST FAIL\n"); return 1;}
 std::printf("009f1730_TEST PASS\n"); return 0;}