#include <cstdio>
struct Sub { void Dtor(int); };
struct S { char pad[4]; Sub* sub; };
static int g_arg=0; void Sub::Dtor(int x){ g_arg=x; }
static void Dest_val(S* self){ self->sub->Dtor(153); }
int main(){ Sub s; S o; o.sub=&s; Dest_val(&o);
 if(g_arg!=153){ std::printf("007c3e90_TEST FAIL\n"); return 1;}
 std::printf("007c3e90_TEST PASS\n"); return 0;}