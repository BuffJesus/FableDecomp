#include "engine/CNavSwitchableLeafNode.h"
#include <cstdio>
static unsigned char f(CNavSwitchableLeafNode* self){ if(!self->m_disabled) return self->b1c; return (unsigned char)0xFF; }
int main(){ CNavSwitchableLeafNode o; o.m_disabled=0; o.b1c=7; if(f(&o)!=7){ std::printf("00a75c10_TEST FAIL\n"); return 1;}
 o.m_disabled=1; if(f(&o)!=0xFF){ std::printf("00a75c10_TEST FAIL\n"); return 1;}
 std::printf("00a75c10_TEST PASS\n"); return 0;}