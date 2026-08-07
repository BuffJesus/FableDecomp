struct S; extern void __fastcall glp_helper(S*);
struct S { char pad[0x44]; unsigned short w44; };
unsigned short __fastcall GetLocationPort(S* self){ glp_helper(self); return self->w44; }