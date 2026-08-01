#include <cstddef>
#include <cstdio>

class CCharString;

class CGameScriptInterface
{
public:
    virtual void vf0();
    void GiveHeroWeapon(const CCharString& s, bool equipNow) const;
};

typedef void (CGameScriptInterface::*GiveHeroWeaponSlot_t)(const CCharString& s, int slot, bool equipNow) const;

struct CGameScriptInterface_vftable
{
    unsigned char pad0[0x1E4];
    void* giveHeroWeaponSlot;
};

void CGameScriptInterface::GiveHeroWeapon(const CCharString& s, bool equipNow) const
{
    const CGameScriptInterface_vftable* vft = *reinterpret_cast<CGameScriptInterface_vftable* const*>(this);
    void* raw = vft->giveHeroWeaponSlot;
    GiveHeroWeaponSlot_t fn;
    *reinterpret_cast<void**>(&fn) = raw;
    (this->*fn)(s, -1, equipNow);
}

static const CGameScriptInterface* g_self = 0;
static const void* g_str = 0;
static int g_slot = 0;
static bool g_equip = false;
static int g_called = 0;

struct CalleeRecorder
{
    void Record(const CCharString& s, int slot, bool equipNow)
    {
        g_self = reinterpret_cast<const CGameScriptInterface*>(this);
        g_str = reinterpret_cast<const void*>(&s);
        g_slot = slot;
        g_equip = equipNow;
        g_called++;
    }
};

int main()
{
    void (CalleeRecorder::*pm)(const CCharString&, int, bool) = &CalleeRecorder::Record;
    void* rawfn = *reinterpret_cast<void**>(&pm);

    CGameScriptInterface_vftable vft;
    vft.giveHeroWeaponSlot = rawfn;
    CGameScriptInterface_vftable* pvft = &vft;

    unsigned char storage[8];
    *reinterpret_cast<CGameScriptInterface_vftable**>(storage) = pvft;
    CGameScriptInterface* obj = reinterpret_cast<CGameScriptInterface*>(storage);

    int fakeString = 0x1234;
    const CCharString& sref = *reinterpret_cast<const CCharString*>(&fakeString);

    obj->GiveHeroWeapon(sref, true);

    bool ok = (g_called == 1) && (g_self == obj) && (g_str == reinterpret_cast<const void*>(&fakeString)) && (g_slot == -1) && (g_equip == true);

    if (ok)
        printf("GIVEHEROWEAPON_OK\n");
    else
        printf("FAIL called=%d slot=%d equip=%d self=%p obj=%p\n", g_called, g_slot, (int)g_equip, (void*)g_self, (void*)obj);
    return 0;
}