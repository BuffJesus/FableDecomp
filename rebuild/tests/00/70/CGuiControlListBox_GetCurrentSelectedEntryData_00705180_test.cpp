#include <cstdio>
struct Entry { char pad[4]; unsigned long data; };
struct S { char pad[0x10]; Entry* cur; };
static unsigned long f(S* self){ Entry* e=self->cur; return e ? e->data : 0; }
int main(){ Entry e; e.data=0xABCD; S obj; obj.cur=&e;
 if(f(&obj)!=0xABCD){ std::printf("00705180_TEST FAIL\n"); return 1;}
 S n; n.cur=0; if(f(&n)!=0){ std::printf("00705180_TEST FAIL\n"); return 1;}
 std::printf("00705180_TEST PASS\n"); return 0;}