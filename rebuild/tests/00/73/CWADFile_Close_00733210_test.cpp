#include "rebuild_abi.h"
#include <cstdio>
struct CWADFile { char pad00[0xc]; long f0c; long f10; void Close(); };
void CWADFile::Close(){ f0c=(long)0xfffffcf7; f10=(long)0xfffffc19; }
int main(){
    CWADFile w; w.f0c=0; w.f10=0; w.Close();
    if(w.f0c!=(long)0xfffffcf7||w.f10!=(long)0xfffffc19){std::printf("BAD\n");return 1;}
    std::printf("WADCLOSE733_OK\n"); return 0;
}