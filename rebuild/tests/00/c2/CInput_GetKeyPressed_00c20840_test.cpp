#include <cstdio>
struct S { char pad[0xAC]; unsigned short k; };
static bool GetKeyPressed(S* self){ return self->k != 0; }
int main(){ S obj; obj.k=0; if(GetKeyPressed(&obj)){ std::printf("00c20840_TEST FAIL\n"); return 1;}
 obj.k=5; if(!GetKeyPressed(&obj)){ std::printf("00c20840_TEST FAIL\n"); return 1;}
 std::printf("00c20840_TEST PASS\n"); return 0;}