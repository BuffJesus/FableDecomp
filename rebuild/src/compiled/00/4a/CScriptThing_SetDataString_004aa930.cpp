struct CCharString; struct Inner { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void VSet(const CCharString&); };
struct S { char pad[4]; Inner* f4; void SetDataString(const CCharString&); };
void S::SetDataString(const CCharString& s){ Inner* p = this->f4; if(p) p->VSet(s); }