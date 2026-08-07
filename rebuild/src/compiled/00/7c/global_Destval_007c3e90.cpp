struct Sub { void Dtor(int); };
struct S { char pad[4]; Sub* sub; };
void __fastcall Dest_val(S* self){ self->sub->Dtor(153); }