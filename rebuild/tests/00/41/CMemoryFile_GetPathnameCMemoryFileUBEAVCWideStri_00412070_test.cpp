#include <cstdio>
struct CWideString { void* p; void ctor(const void* src){ p = (void*)src; } };
struct CMemoryFile { char pad[0x20]; char member[4]; };
CWideString* CMemoryFile_GetPathname(const CMemoryFile* self, void* edx, CWideString* ret)
{
    ret->ctor((const char*)self + 0x20);
    return ret;
}
int main(){
    CMemoryFile mf;
    CWideString out; out.p = 0;
    CWideString* r = CMemoryFile_GetPathname(&mf, 0, &out);
    if (r != &out) { std::printf("FAIL ret ptr\n"); return 1; }
    if (out.p != (const char*)&mf + 0x20) { std::printf("FAIL src offset\n"); return 1; }
    std::printf("CMemoryFile_00412070_TEST PASS\n");
    return 0;
}