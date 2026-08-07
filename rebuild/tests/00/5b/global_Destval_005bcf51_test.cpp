#include <cstdio>
static int g_flag=-1;
struct T { void DtorHelper(int flag); };
void T::DtorHelper(int flag){ g_flag=flag; }
static void Dest_val(T* self){ self->DtorHelper(1); }
int main(){ T o; Dest_val(&o); if(g_flag!=1){ std::printf("005bcf51_TEST FAIL\n"); return 1;}
 std::printf("005bcf51_TEST PASS\n"); return 0;}