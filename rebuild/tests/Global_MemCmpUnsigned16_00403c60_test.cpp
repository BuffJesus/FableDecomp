#include <stdio.h>
int __cdecl MemCmp_Unsigned16(const unsigned short*,const unsigned short*,unsigned long);
int main()
{
    unsigned short a[4]={1,2,3,4}; unsigned short b[4]={1,2,3,4};
    if(MemCmp_Unsigned16(a,b,0)!=0 || MemCmp_Unsigned16(a,b,4)!=0)return 1;
    b[2]=9; if(MemCmp_Unsigned16(a,b,4)!=-1)return 2;
    b[2]=0; if(MemCmp_Unsigned16(a,b,4)!=1)return 3;
    printf("MEMCMP_UNSIGNED16_TEST PASS\n"); return 0;
}
