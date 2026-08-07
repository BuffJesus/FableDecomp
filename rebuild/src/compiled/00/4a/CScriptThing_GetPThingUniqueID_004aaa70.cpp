struct P { long long (__fastcall **v)(P*); };
struct S { char pad[4]; P* f4; long long GetPThingUniqueID(); };
long long S::GetPThingUniqueID(){ P* p=this->f4; if(!p) return 0; return p->v[0x30/4](p); }