#include <cstdio>
static char __fastcall AreAllowedToCoexist(int actionA, int actionB){
    switch (actionA) {
    case 0x08: case 0x1f: case 0x2d:
        if (actionB == 0x08 || actionB == 0x1f || actionB == 0x2d) return 1;
        return 0;
    case 0x1a: case 0x56:
        if (actionB == 0x1a || actionB == 0x56) return 1;
        return 0; }
    return 0;
}
int main(){
 if(!AreAllowedToCoexist(0x08,0x1f)){std::printf("005578a0_TEST FAIL a\n");return 1;}
 if(!AreAllowedToCoexist(0x2d,0x08)){std::printf("005578a0_TEST FAIL b\n");return 1;}
 if(AreAllowedToCoexist(0x08,0x1a)){std::printf("005578a0_TEST FAIL c\n");return 1;}
 if(!AreAllowedToCoexist(0x1a,0x56)){std::printf("005578a0_TEST FAIL d\n");return 1;}
 if(AreAllowedToCoexist(0x56,0x2d)){std::printf("005578a0_TEST FAIL e\n");return 1;}
 if(AreAllowedToCoexist(0x09,0x09)){std::printf("005578a0_TEST FAIL f\n");return 1;}
 if(AreAllowedToCoexist(0x100,0x08)){std::printf("005578a0_TEST FAIL g\n");return 1;}
 std::printf("005578a0_TEST PASS\n"); return 0;}