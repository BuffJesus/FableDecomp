struct CBackgroundTesselator
{
    void Update();
};

struct CEngineLandscapeMap
{
    int m_pad0;                    // +0x00
    CBackgroundTesselator* m_tess; // +0x04
    char m_fill[0x54];             // +0x08 .. +0x5b
    char m_updating;               // +0x5c
    void UpdateBackgroundTesselation();
};

void CEngineLandscapeMap::UpdateBackgroundTesselation()
{
    CBackgroundTesselator* t = this->m_tess;
    this->m_updating = 1;
    if (t)
        t->Update();
    this->m_updating = 0;
}