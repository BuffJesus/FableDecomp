struct Sub; extern void __fastcall sub(Sub* p);
struct S { char pad[0x70]; int cnt; void M(); };
void S::M(){ sub((Sub*)((char*)this+0x1c)); this->cnt++; }