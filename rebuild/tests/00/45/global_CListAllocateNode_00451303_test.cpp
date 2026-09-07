#include <cstdio>
#include <cstdlib>
static void** __fastcall Run(void** out,void*,void*){*out=0;*out=std::malloc(0x14);return out;}int main(){void* node=(void*)1;void** returned=Run(&node,0,0);if(returned!=&node||node==0)return 1;unsigned char* bytes=(unsigned char*)node;for(int i=0;i<0x14;++i)bytes[i]=(unsigned char)i;std::free(node);std::printf("CLIST_ALLOCATE_PASS\n");return 0;}