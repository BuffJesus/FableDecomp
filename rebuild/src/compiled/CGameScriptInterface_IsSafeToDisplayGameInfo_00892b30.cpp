struct CSub { bool Query(bool flag); };
struct CObj { char pad[0x18]; CSub sub; };
extern CObj* g_obj;

bool __fastcall CGameScriptInterface_IsSafeToDisplayGameInfo(void* self)
{
    return g_obj->sub.Query(false);
}