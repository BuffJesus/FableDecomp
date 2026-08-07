#include <cstdio>
int g_count=5;
struct S { char pad[4]; unsigned char b4; void M(); };
void S::M(){ if(this->b4==1){ --g_count; this->b4=0; } }
int main(){ S o; o.b4=1; o.M(); if(g_count!=4||o.b4!=0){ std::printf("0099a360_TEST FAIL\n"); return 1;}
 o.b4=2; o.M(); if(g_count!=4){ std::printf("0099a360_TEST FAIL\n"); return 1;}
 std::printf("0099a360_TEST PASS\n"); return 0;}