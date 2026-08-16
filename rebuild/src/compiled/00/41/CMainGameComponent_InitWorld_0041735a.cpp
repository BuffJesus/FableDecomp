struct CGameDefinitionManager;
struct WorldCtx;
struct WorldObj {
    virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();
    virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void vtbl9();
    WorldObj* Construct(WorldCtx* ctx);
};
struct WorldCtx { void* comp; int z4; void* f1c; CGameDefinitionManager* defmgr; int z10; };
struct CCharString2 { char buf[4]; CCharString2(int lit, int n); ~CCharString2(); };
struct GObj { void* GetPtr(); };
struct CountedPtr { WorldObj* ptr; void DeleteData(WorldObj* w); };
extern "C" CGameDefinitionManager& __fastcall GetDefMgr();
extern "C" void __fastcall DisplayProgress(CCharString2* text, bool edxBool, float progress, int flags);
extern GObj g_obj;
extern unsigned char g_flag1, g_flag2;
struct CMainGameComponent {
    char pad0[0x1c]; void* f1c; char pad20[0x24-0x20]; CountedPtr f24;
    bool InitEventPackageLoading(void* p);
    bool InitEventPackageSaving(void* p);
    void InitWorld();
};
void CMainGameComponent::InitWorld()
{
    WorldCtx ctx;
    ctx.z4 = 0; ctx.defmgr = 0; ctx.z10 = 0;
    ctx.comp = this;
    ctx.f1c = this->f1c;
    ctx.defmgr = &GetDefMgr();
    WorldObj* w = (WorldObj*)::operator new(0x198);
    if (w) w = w->Construct(&ctx);
    else w = 0;
    this->f24.DeleteData(w);
    if (g_flag1) this->InitEventPackageLoading(g_obj.GetPtr());
    if (g_flag2) this->InitEventPackageSaving(g_obj.GetPtr());
    {
        CCharString2 s(0x122eea4, -1);
        DisplayProgress(&s, false, 0.5f, 0);
    }
    this->f24.ptr->vtbl9();
}
