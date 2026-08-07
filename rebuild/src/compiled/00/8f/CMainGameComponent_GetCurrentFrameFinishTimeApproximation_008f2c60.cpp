struct S; extern double __fastcall gcf_helper(S*);
struct S { char pad[0x118]; float f118; };
double __fastcall GetCurrentFrameFinishTimeApproximation(S* self){ return gcf_helper(self) * self->f118; }