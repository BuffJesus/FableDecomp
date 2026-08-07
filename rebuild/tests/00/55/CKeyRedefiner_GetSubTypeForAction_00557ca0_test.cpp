#include <cstdio>
static int g_subtypeCounterA=0, g_subtypeCounterB=0, g_hret=0;
static int __fastcall GetSubTypeAnalogDirection(void*){ return g_hret; }
static int __stdcall GetSubTypeForAction(int* record){
    switch (*record) {
    case 0x37: return ++g_subtypeCounterA;
 case 0x38: return ++g_subtypeCounterB;
    case 0x3c: switch (GetSubTypeAnalogDirection(record)) {
        case 1: return 0xa; case 2: return 0xb; case 3: return 0xc; case 4: return 0xd; } }
    return 0;
}
int main(){ int r;
 r=0x37; if(GetSubTypeForAction(&r)!=1||GetSubTypeForAction(&r)!=2){std::printf("00557ca0_TEST FAIL a\n");return 1;}
 r=0x38; if(GetSubTypeForAction(&r)!=1){std::printf("00557ca0_TEST FAIL b\n");return 1;}
 r=0x3c; g_hret=1; if(GetSubTypeForAction(&r)!=0xa){std::printf("00557ca0_TEST FAIL c\n");return 1;}
 g_hret=4; if(GetSubTypeForAction(&r)!=0xd){std::printf("00557ca0_TEST FAIL d\n");return 1;}
 g_hret=5; if(GetSubTypeForAction(&r)!=0){std::printf("00557ca0_TEST FAIL e\n");return 1;}
 r=0x99; if(GetSubTypeForAction(&r)!=0){std::printf("00557ca0_TEST FAIL f\n");return 1;}
 std::printf("00557ca0_TEST PASS\n"); return 0;}