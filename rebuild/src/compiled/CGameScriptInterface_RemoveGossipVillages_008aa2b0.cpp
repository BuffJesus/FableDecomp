
struct Str {
    char* p;
    Str(const Str& o);
    ~Str();
};

struct GObj {
    void RemoveGV(Str* s);
};

void __fastcall CGameScriptInterface_RemoveGossipVillages(void* self, Str param)
{
    Str tmp(param);
    ((GObj*)0x13bae50)->RemoveGV(&tmp);
}