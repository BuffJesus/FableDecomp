struct CResult {
    bool Method2(int n);
};
struct CSub {
    CResult* GetResult();
};
struct CGlobal {
    char pad[0x1c];
    CSub* sub;   // at +0x1c
};

extern CGlobal* g_global;

int __fastcall CGameScriptInterface_IsHeroInTavernGame(void* self)
{
    (void)self;
    CResult* r = g_global->sub->GetResult();
    if (r && !r->Method2(0x1a))
        return 0;
    return 1;
}