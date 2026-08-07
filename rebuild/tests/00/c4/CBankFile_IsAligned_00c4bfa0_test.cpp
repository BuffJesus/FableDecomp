#include "rebuild_abi.h"
#include <cstdio>
struct CBankFile { char pad00[0xc]; unsigned int m_alignment; bool IsAligned(unsigned int n); };
bool CBankFile::IsAligned(unsigned int n){ return (n % m_alignment)==0; }
int main(){
    CBankFile b; b.m_alignment=16;
    if(!b.IsAligned(32)){std::printf("B1\n");return 1;}
    if(b.IsAligned(33)){std::printf("B2\n");return 1;}
    std::printf("ISALIGNED_OK\n"); return 0;
}