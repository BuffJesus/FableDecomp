#include <cstdio>
struct CRegion;
struct S { char pad[0x2c]; char* f2c; CRegion& GetRegion(long i); };
CRegion& S::GetRegion(long i){ return *(CRegion*)(this->f2c + i*0x58); }
int main(){ char buf[0x200]; S o; o.f2c=buf;
 if((char*)&o.GetRegion(2)!=buf+2*0x58){ std::printf("004fc180_TEST FAIL\n"); return 1;}
 std::printf("004fc180_TEST PASS\n"); return 0;}