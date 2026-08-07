extern void* __fastcall dv_h1(void*);
extern void __fastcall dv_h2(void*);
void __fastcall Dest_val(void* self){ dv_h2(dv_h1(reinterpret_cast<char*>(self)+8)); }