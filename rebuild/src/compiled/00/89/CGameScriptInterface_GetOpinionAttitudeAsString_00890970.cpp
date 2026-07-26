extern int g_attTable[];

struct StrDest {
    void ctor(const int* e);
};

void __stdcall CGameScriptInterface_GetOpinionAttitudeAsString(int attitude, StrDest* dest)
{
    dest->ctor(&g_attTable[attitude]);
}