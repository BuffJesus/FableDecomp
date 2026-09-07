#include <cstdio>
#include <cstdlib>
static void** __fastcall Run(void** out,void*,void*){*(unsigned long*)out&=0;*out=std::malloc(0x18);return out;}int main(){void* node=(void*)1;if(Run(&node,0,0)!=&node||!node)return 1;((unsigned char*)node)[0x17]=0x5a;std::free(node);std::printf("ALLOC22_0057932f_PASS\n");return 0;}