#include "fable_date_and_time.h"
#include <stdio.h>
#include <string.h>
int main(){
 unsigned long a[7]={0xffffffff,2,3,4,5,6,0x8000ffff},b[7];memcpy(b,a,sizeof(a));
 const CDateAndTime& left=*reinterpret_cast<const CDateAndTime*>(a);const CDateAndTime& right=*reinterpret_cast<const CDateAndTime*>(b);
 if(!(left==right) || !(left==left))return 1;
 for(unsigned i=0;i<7;++i){b[i]^=0x80000000;if(left==right || right==left)return 2;b[i]^=0x80000000;}
 if(memcmp(a,b,sizeof(a)))return 3;
 puts("DATE_EQUAL_009e9ad0_PASS");return 0;
}
