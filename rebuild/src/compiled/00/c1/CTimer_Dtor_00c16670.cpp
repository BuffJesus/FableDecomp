struct T; struct Mgr { virtual void d0(); virtual void Unreg(T*); };
extern Mgr* g_mgr;
void __fastcall Dtor(T* self){ g_mgr->Unreg(self); }