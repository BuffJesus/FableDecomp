#include <stdio.h>
#include <string.h>
#include "fable_surface_types.h"
extern "C" bool __fastcall Equivalent(const CPixelFormat*,void*,const CPixelFormat&);
#pragma comment(linker,"/alternatename:@Equivalent@12=?IsEquivalent@CPixelFormat@@QBE_NABV1@@Z")
int main(){const unsigned values[]={0,1,2,5,17,23,45,46,0x7fffffff,0x80000000,0xffffffff,0xfffffffe,0x12345678,0x87654321,0x10000,0x100};unsigned cases=0;
for(unsigned a=0;a<16;++a)for(unsigned b=0;b<17;++b){CPixelFormat left,right;memcpy(&left.FormatIndex,&values[a],4);memcpy(&right.FormatIndex,&values[b==16?a:b],4);const CPixelFormat& other=b==16?left:right;const bool expected=b==16 || a==b;unsigned beforeLeft,beforeRight;memcpy(&beforeLeft,&left,4);memcpy(&beforeRight,&right,4);if(left.IsEquivalent(other)!=expected || Equivalent(&left,0,other)!=expected || memcmp(&left,&beforeLeft,4) || memcmp(&right,&beforeRight,4))return 80;++cases;}
printf("PIXEL_FORMAT_EQUIVALENT_PASS cases=%u paths=%u\n",cases,cases*2);return cases==272?0:80;}
