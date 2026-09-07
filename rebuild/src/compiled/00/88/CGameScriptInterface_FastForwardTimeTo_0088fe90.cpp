class CGameTimeManager
{
public:
    void FastForwardTimeTo(float timeOfDay, float speed);
};

class CGameScriptInterface
{
public:
    void FastForwardTimeTo(float timeOfDay, float speed) const;
};

struct SelfOverlay
{
    unsigned char pad00[0x04];
    void* field04;
};

struct Field04Overlay
{
    unsigned char pad00[0x1C];
    CGameTimeManager* field1C;
};

#define g_FastForwardTimeTo_WrapValue (*reinterpret_cast<volatile float*>(0x0123822C))
#define g_FastForwardTimeTo_TimeScale (*reinterpret_cast<float*>(0x0122DC88))

void CGameScriptInterface::FastForwardTimeTo(
    float timeOfDay,
    float speed) const
{
    while (g_FastForwardTimeTo_WrapValue <= timeOfDay)
    {
        timeOfDay -= g_FastForwardTimeTo_WrapValue;
    }

    const SelfOverlay* self = (const SelfOverlay*)this;
    const Field04Overlay* field04 = (const Field04Overlay*)self->field04;
    field04->field1C->FastForwardTimeTo(
        timeOfDay * g_FastForwardTimeTo_TimeScale,
        speed);
}