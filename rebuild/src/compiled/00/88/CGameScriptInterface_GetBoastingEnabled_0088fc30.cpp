extern bool g_boastingEnabled;

bool __fastcall CGameScriptInterface_GetBoastingEnabled(void* self)
{
    (void)self;
    return g_boastingEnabled;
}