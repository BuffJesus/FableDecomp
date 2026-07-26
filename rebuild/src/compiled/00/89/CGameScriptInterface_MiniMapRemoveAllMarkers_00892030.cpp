struct CObj; struct CSub;
extern CObj* g_obj;
extern "C" CSub* __fastcall get_sub(CObj* self);
extern "C" void __fastcall sub_method(CSub* self);

void __fastcall CGameScriptInterface_MiniMapRemoveAllMarkers(void* self)
{
    (void)self;
    CSub* s = get_sub(g_obj);
    sub_method(s);
}