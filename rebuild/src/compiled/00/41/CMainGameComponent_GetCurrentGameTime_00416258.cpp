struct S; extern double __fastcall ggt_helper(S*);
struct S { char pad[0x161a8]; double d161a8; };
double __fastcall GetCurrentGameTime(S* self){ return ggt_helper(self) - self->d161a8; }