#include <cstdio>

class C3DVector { public: float x, y, z; };

class CWorldMap
{
public:
    float GetWaterSizeZAt(const C3DVector& position) const;
};

float CWorldMap::GetWaterSizeZAt(const C3DVector& position) const
{
    return position.z + 100.0f;
}

class CWorldMapProvider
{
public:
    virtual void slot00() {}
    virtual void slot01() {}
    virtual void slot02() {}
    virtual void slot03() {}
    virtual void slot04() {}
    virtual void slot05() {}
    virtual void slot06() {}
    virtual void slot07() {}
    virtual void slot08() {}
    virtual void slot09() {}
    virtual void slot10() {}
    virtual void slot11() {}
    virtual void slot12() {}
    virtual CWorldMap* GetWorldMap();
};

static CWorldMap g_world;

CWorldMap* CWorldMapProvider::GetWorldMap()
{
    return &g_world;
}

class CGameScriptInterface
{
public:
    float GetWaterHeightAtPosition(const C3DVector& position) const;
};

struct CGameScriptInterface_Overlay
{
    CWorldMapProvider* m_Provider0x00;
    CWorldMapProvider* m_Provider0x04;
};

float CGameScriptInterface::GetWaterHeightAtPosition(const C3DVector& position) const
{
    const CGameScriptInterface_Overlay* const self =
        reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    return self->m_Provider0x04->GetWorldMap()->GetWaterSizeZAt(position);
}

int main()
{
    CWorldMapProvider provider;
    CGameScriptInterface_Overlay overlay;
    overlay.m_Provider0x00 = 0;
    overlay.m_Provider0x04 = &provider;

    C3DVector pos; pos.x = 1.0f; pos.y = 2.0f; pos.z = 7.0f;

    CGameScriptInterface* gsi = reinterpret_cast<CGameScriptInterface*>(&overlay);
    float h = gsi->GetWaterHeightAtPosition(pos);

    if (h == 107.0f)
        printf("WATER_HEIGHT_OK %f\n", h);
    else
        printf("WATER_HEIGHT_FAIL %f\n", h);
    return 0;
}