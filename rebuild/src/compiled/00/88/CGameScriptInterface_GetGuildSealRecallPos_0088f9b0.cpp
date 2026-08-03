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