#include <cstdio>
static void* g_d=0;
struct CDataOutputStream { void Write(void* d); };
void CDataOutputStream::Write(void* d){ g_d=d; }
struct S { char pad[8]; void* f8; void TransferBinaryOut(CDataOutputStream* s); };
void S::TransferBinaryOut(CDataOutputStream* s){ s->Write(this->f8); }
int main(){ int m; S o; o.f8=&m; CDataOutputStream s; o.TransferBinaryOut(&s);
 if(g_d!=&m){ std::printf("005db330_TEST FAIL\n"); return 1;}
 std::printf("005db330_TEST PASS\n"); return 0;}