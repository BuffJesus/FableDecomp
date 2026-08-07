struct Inner { void (__stdcall **vtbl)(Inner*, unsigned long); };
struct S { Inner* f0; void Unlock(unsigned long v); };
void S::Unlock(unsigned long v){ Inner* p=this->f0; p->vtbl[0x50/4](p, v); }