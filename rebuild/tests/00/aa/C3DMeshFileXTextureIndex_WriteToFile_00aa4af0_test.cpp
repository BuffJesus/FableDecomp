#include <cstdio>
static void* g_x=0; static char* g_this=0;
struct Arg { void N(void* x); };
struct S { char pad[0x10]; void* f10; void WriteToFile(Arg* a, int b); };
void Arg::N(void* x){ g_this=(char*)this; g_x=x; }
void S::WriteToFile(Arg* a, int b){ a->N(this->f10); }
int main(){ Arg ar; S o; o.f10=(void*)77; o.WriteToFile(&ar,0);
 if(g_this!=(char*)&ar||g_x!=(void*)77){ std::printf("00aa4af0_TEST FAIL\n"); return 1;}
 std::printf("00aa4af0_TEST PASS\n"); return 0;}