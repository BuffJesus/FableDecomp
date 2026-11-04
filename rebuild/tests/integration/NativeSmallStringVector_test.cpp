#include "fable_string.h"
#include "fable_small_vector.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
fable_i32 g_CCharStringInstanceCount_013BD800=16;
static CCharStringData refs[16];
static void require(bool ok){if(!ok){fprintf(stderr,"SMALL_STRING_VECTOR_FAILURE\n");exit(80);}}
struct Block {unsigned char* p;unsigned n;bool freed;};static Block blocks[16];static unsigned used;
void* __cdecl operator new[](size_t n){require(n>=4&&n<=1024&&used<16);unsigned char* p=(unsigned char*)malloc(n+32);require(p!=0);memset(p,0x7d,n+32);Block& b=blocks[used++];b.p=p+16;b.n=n;b.freed=false;return b.p;}
void __cdecl operator delete[](void* p){require(p!=0);Block* b=0;for(unsigned i=0;i<used;++i)if(blocks[i].p==p&&!blocks[i].freed)b=&blocks[i];require(b!=0);for(int i=0;i<16;++i)require(b->p[i-16]==0x7d&&b->p[b->n+i]==0x7d);require(*(unsigned*)p==(b->n-4)/4);for(unsigned i=4;i<b->n;i+=4)require(*(unsigned*)(b->p+i)==0);b->freed=true;free(b->p-16);}
void __cdecl operator delete(void*){require(false);}
static unsigned refIndex(void* p){unsigned value=(unsigned)p,first=(unsigned)refs;require(value>=first&&value<first+sizeof(refs)&&(value-first)%sizeof(refs[0])==0);return (value-first)/sizeof(refs[0]);}
static void initialize(){used=0;require(g_CCharStringInstanceCount_013BD800==16);for(int i=0;i<16;++i){memset(&refs[i],0,sizeof(refs[i]));refs[i].owners=1;}}
static void populate(CSmallVector<CCharString,8>& a,unsigned n,int mode){for(unsigned i=0;i<n;++i){void* ref=mode==0?0:&refs[mode==1?0:i%16];a.Data[i]=*(CCharString*)&ref;}}
static void finished(){require(g_CCharStringInstanceCount_013BD800==16);for(int i=0;i<16;++i)require(refs[i].owners==1);for(unsigned i=0;i<used;++i)require(blocks[i].freed);}
int main(){int cases=0;unsigned sizes[]={0,1,2,7,31,127,128,255},oldSizes[]={0,1,3,7};
 for(int old=0;old<4;++old)for(int n=0;n<8;++n)for(int mode=0;mode<3;++mode){initialize();CSmallVector<CCharString,8> a;a.Data=0;a.Size=0;a.Allocate(oldSizes[old]);populate(a,oldSizes[old],mode);a.Allocate(sizes[n]);require((unsigned char)a.Size==(unsigned char)sizes[n]&&g_CCharStringInstanceCount_013BD800==16+sizes[n]);for(unsigned i=0;i<sizes[n];++i)require(*(void**)&a.Data[i]==0);a.Clear();require(a.Data==0&&a.Size==0);finished();++cases;}
 for(int old=0;old<4;++old)for(int n=0;n<6;++n)for(int mode=0;mode<3;++mode)for(int self=0;self<2;++self){initialize();CSmallVector<CCharString,8> a,b;a.Data=b.Data=0;a.Size=b.Size=0;a.Allocate(oldSizes[old]);populate(a,oldSizes[old],mode);b.Allocate(sizes[n]);populate(b,sizes[n],mode);CSmallVector<CCharString,8>& target=self?b:a;target.Copy(b);require(target.Size==sizes[n]);
  unsigned owners[16];for(int j=0;j<16;++j)owners[j]=1;
  for(unsigned i=0;i<(unsigned char)a.Size;++i){void* p=*(void**)&a.Data[i];if(p)++owners[refIndex(p)];}
  for(unsigned i=0;i<(unsigned char)b.Size;++i){void* p=*(void**)&b.Data[i];if(p)++owners[refIndex(p)];}
  for(unsigned i=0;i<sizes[n];++i){void* expected=self||mode==0?0:&refs[mode==1?0:i%16];require(*(void**)&target.Data[i]==expected);}
  for(int j=0;j<16;++j)require(refs[j].owners==owners[j]);require(g_CCharStringInstanceCount_013BD800==16+(unsigned char)a.Size+(unsigned char)b.Size);a.Clear();b.Clear();finished();++cases;
 }
 printf("SMALL_STRING_VECTOR_PASS cases=%d\n",cases);return 0;
}
