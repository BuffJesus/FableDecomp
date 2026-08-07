#include <cstdio>
struct CInputEvent { unsigned char b0; };
struct S { char pad[0xd14]; int fd14; void EventScanEnd(CInputEvent* e); };
void S::EventScanEnd(CInputEvent* e){ e->b0 = 0; --this->fd14; }
int main(){ CInputEvent e; e.b0=9; S o; o.fd14=5; o.EventScanEnd(&e);
 if(e.b0!=0||o.fd14!=4){ std::printf("009e41c0_TEST FAIL\n"); return 1;}
 std::printf("009e41c0_TEST PASS\n"); return 0;}