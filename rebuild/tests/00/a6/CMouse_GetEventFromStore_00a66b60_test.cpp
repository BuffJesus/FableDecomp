#include <cstdio>
struct CInputEvent; struct CMouse { CInputEvent* GetEventFromStore(unsigned long i); };
CInputEvent* CMouse::GetEventFromStore(unsigned long i){ return (CInputEvent*)((char*)this + i*0x34 + 4); }
int main(){ CMouse o; char* b=(char*)&o; if((char*)o.GetEventFromStore(2)!=b+2*0x34+4){ std::printf("00a66b60_TEST FAIL\n"); return 1;}
 std::printf("00a66b60_TEST PASS\n"); return 0;}