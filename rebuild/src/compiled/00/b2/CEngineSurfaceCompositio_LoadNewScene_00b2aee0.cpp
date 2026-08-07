struct Sub; extern void __fastcall sub(Sub* p);
struct S { char pad[0x31c]; char M(int a); };
char S::M(int a){ sub((Sub*)((char*)this+0x31c)); return 0; }