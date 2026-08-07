struct T { void DtorHelper(int flag); };
void __fastcall Dest_val(T* self){ self->DtorHelper(1); }