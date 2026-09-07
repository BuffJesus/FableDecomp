class CThingDrawInformation
{
public:
    void SetBarPercentage(float percentage);
};

class CGameScriptInterface
{
public:
    void SetTimeOfDay(float timeOfDay) const;
};

struct SelfOverlay
{
    unsigned char pad00[0x04];
    void* field04;
};

struct Field04Overlay
{
    unsigned char pad00[0x1C];
    CThingDrawInformation* field1C;
};

#define g_SetTimeOfDay_WrapValue (*reinterpret_cast<volatile float*>(0x0123822C))
#define g_SetTimeOfDay_BarScale (*reinterpret_cast<float*>(0x0122DC88))

void CGameScriptInterface::SetTimeOfDay(float timeOfDay) const
{
    while (g_SetTimeOfDay_WrapValue <= timeOfDay)
    {
        timeOfDay -= g_SetTimeOfDay_WrapValue;
    }

    const SelfOverlay* self = (const SelfOverlay*)this;
    const Field04Overlay* field04 = (const Field04Overlay*)self->field04;
    field04->field1C->SetBarPercentage(
        timeOfDay * g_SetTimeOfDay_BarScale);
}