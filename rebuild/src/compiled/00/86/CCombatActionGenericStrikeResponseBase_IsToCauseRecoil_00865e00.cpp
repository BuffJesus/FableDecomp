extern void* __fastcall r_h1(void*);
extern bool __fastcall r_h2(void*);
bool __fastcall IsToCauseRecoil(void* self){ return r_h2(r_h1(reinterpret_cast<char*>(self)+8)); }