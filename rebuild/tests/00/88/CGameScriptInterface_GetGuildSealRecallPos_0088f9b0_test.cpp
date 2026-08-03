#include <cstdio>

class C3DVector
{
public:
    float x;
    float y;
    float z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};

class CGameScriptInterface
{
public:
    C3DVector GetGuildSealRecallPos() const;
};

struct CGameScriptInterface_Overlay
{
    char pad_0x00[0x04];
    void* field_0x04;
};

struct GuildSealRecallPosSource_Overlay
{
    char pad_0x00[0x154];
    C3DVector field_0x154;
};

C3DVector CGameScriptInterface::GetGuildSealRecallPos() const
{
    const GuildSealRecallPosSource_Overlay* const source =
        reinterpret_cast<const GuildSealRecallPosSource_Overlay*>(
            reinterpret_cast<const CGameScriptInterface_Overlay*>(this)->field_0x04);
    return source->field_0x154;
}

int main()
{
    GuildSealRecallPosSource_Overlay src;
    src.field_0x154.x = 12.5f;
    src.field_0x154.y = -3.25f;
    src.field_0x154.z = 7.75f;

    CGameScriptInterface_Overlay iface;
    iface.field_0x04 = &src;

    const CGameScriptInterface* gsi = reinterpret_cast<const CGameScriptInterface*>(&iface);
    C3DVector result = gsi->GetGuildSealRecallPos();

    if (result.x == 12.5f && result.y == -3.25f && result.z == 7.75f)
    {
        printf("GUILD_SEAL_RECALL_POS_OK\n");
        return 0;
    }
    printf("FAIL x=%f y=%f z=%f\n", result.x, result.y, result.z);
    return 1;
}