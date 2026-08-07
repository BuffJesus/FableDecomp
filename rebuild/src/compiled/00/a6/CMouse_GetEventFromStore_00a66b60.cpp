struct CInputEvent; struct CMouse { CInputEvent* GetEventFromStore(unsigned long i); };
CInputEvent* CMouse::GetEventFromStore(unsigned long i){ return (CInputEvent*)((char*)this + i*0x34 + 4); }