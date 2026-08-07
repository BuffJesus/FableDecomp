struct S { unsigned int (__fastcall** vtbl)(S*); char pad[0x18-4]; unsigned int f18; int M(); };
int S::M(){ unsigned int r=this->vtbl[0x28/4](this); return (unsigned int)r < this->f18 ? 1 : 0; }