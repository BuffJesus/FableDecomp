struct S; extern void __fastcall h1(S* self); extern void __fastcall h2(S* self);
struct S { int x; };
void __fastcall Close(S* self){ h1(self); h2(self); }