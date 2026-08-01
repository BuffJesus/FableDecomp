class C3DVector;

class CWorldMap
{
public:
    float GetWaterSizeZAt(const C3DVector& position) const;
};

class CWorldMapProvider
{
public:
    virtual void slot00();
    virtual void slot01();
    virtual void slot02();
    virtual void slot03();
    virtual void slot04();
    virtual void slot05();
    virtual void slot06();
    virtual void slot07();
    virtual void slot08();
    virtual void slot09();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual CWorldMap* GetWorldMap();
};

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