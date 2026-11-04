#include "fable_string.h"
#include <stdio.h>
#include <string.h>
static unsigned long word(const unsigned char* p){return p[0]|((unsigned long)p[1]<<8)|((unsigned long)p[2]<<16)|((unsigned long)p[3]<<24);}
int main(){int cases=0;
 for(unsigned n=0;n<18;++n)for(unsigned m=0;m<18;++m)for(unsigned mode=0;mode<8;++mode){
  unsigned char a[32],b[32];for(unsigned i=0;i<32;++i)a[i]=b[i]=(unsigned char)(i*71+n*13);
  if(mode==4)b[mode%4]^=0x80;if(mode==5)b[16]^=0xff;if(mode==6)b[m]^=0x80;if(mode==7)b[0]^=0xff;
  CCharStringData left,right;left.Data.PData=(char*)a;right.Data.PData=(char*)b;left.Data.StringLength=n;right.Data.StringLength=m;
  void* l=mode==0?0:&left;void* r=mode==1?0:mode==2?l:&right;
  unsigned char beforeA[32],beforeB[32];memcpy(beforeA,a,32);memcpy(beforeB,b,32);
  bool expected=false;
  if(l!=r){if(!l)expected=true;else if(r){unsigned ll=((CCharStringData*)l)->Data.StringLength,rr=((CCharStringData*)r)->Data.StringLength;
   if(ll!=rr)expected=ll<rr;else for(unsigned j=0;j<ll;j+=4){unsigned long x=word((unsigned char*)((CCharStringData*)l)->Data.PData+j),y=word((unsigned char*)((CCharStringData*)r)->Data.PData+j);if(x!=y){expected=x<y;break;}}
  }}
  if(((CCharString*)&l)->FastCompare(*(CCharString*)&r)!=expected||memcmp(a,beforeA,32)||memcmp(b,beforeB,32))return 1;++cases;
 }
 printf("STRING_FAST_COMPARE_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
