#include <cstdio>
struct CTextureManager { unsigned long GetPoolTextureSize(unsigned long i); };
unsigned long CTextureManager::GetPoolTextureSize(unsigned long i){ return *(unsigned long*)((char*)this + i*4 + 0x120); }
int main(){ char blk[0x200]; *(unsigned long*)(blk+2*4+0x120)=0xCAFE; CTextureManager* o=(CTextureManager*)blk;
 if(o->GetPoolTextureSize(2)!=0xCAFE){ std::printf("00b244f0_TEST FAIL\n"); return 1;}
 std::printf("00b244f0_TEST PASS\n"); return 0;}