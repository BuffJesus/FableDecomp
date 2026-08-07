struct CInputEvent { unsigned char b0; };
struct S { char pad[0xd14]; int fd14; void EventScanEnd(CInputEvent* e); };
void S::EventScanEnd(CInputEvent* e){ e->b0 = 0; --this->fd14; }