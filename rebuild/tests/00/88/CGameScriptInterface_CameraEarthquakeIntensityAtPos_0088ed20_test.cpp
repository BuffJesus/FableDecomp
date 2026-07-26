#include <cstdio>

struct C3DVector
{
    float x;
    float y;
    float z;
};

extern const float CGSI_Rumble_Zero;
extern const float CGSI_Rumble_One;
extern const float CGSI_Rumble_Hundred;
extern void __fastcall CGSI_Rumble_Create(const C3DVector*, unsigned char, float, float);
void __fastcall CGameScriptInterface_CameraEarthquakeIntensityAtPos(const void*, unsigned long, const C3DVector&, float, float);

const float CGSI_Rumble_Zero = 0.0f;
const float CGSI_Rumble_One = 1.0f;
const float CGSI_Rumble_Hundred = 100.0f;
static const C3DVector* g_position;
static unsigned char g_localOnly;
static float g_intensity;
static float g_radius;

void __fastcall CGSI_Rumble_Create(const C3DVector* position, unsigned char localOnly, float intensity, float radius)
{
    g_position = position;
    g_localOnly = localOnly;
    g_intensity = intensity;
    g_radius = radius;
}

void __fastcall CGameScriptInterface_CameraEarthquakeIntensityAtPos(const void*, unsigned long, const C3DVector& position, float intensity, float radius)
{
    if (intensity < CGSI_Rumble_Zero)
        intensity = 0.0f;
    else if (intensity > CGSI_Rumble_One)
        intensity = 1.0f;
    if (radius < CGSI_Rumble_Zero)
        radius = 0.0f;
    else if (radius > CGSI_Rumble_Hundred)
        radius = 100.0f;
    CGSI_Rumble_Create(&position, 0, intensity, radius);
}

static int Check(const C3DVector& position, float intensity, float radius, float expectedIntensity, float expectedRadius)
{
    CGameScriptInterface_CameraEarthquakeIntensityAtPos(0, 0, position, intensity, radius);
    return g_position == &position && g_localOnly == 0 && g_intensity == expectedIntensity && g_radius == expectedRadius;
}

int main()
{
    C3DVector position = { 1.0f, 2.0f, 3.0f };
    if (!Check(position, -2.0f, -5.0f, 0.0f, 0.0f)) return 1;
    if (!Check(position, 0.5f, 25.0f, 0.5f, 25.0f)) return 2;
    if (!Check(position, 2.0f, 125.0f, 1.0f, 100.0f)) return 3;
    std::printf("FSE2_0088ed20_TEST PASS\n");
    return 0;
}