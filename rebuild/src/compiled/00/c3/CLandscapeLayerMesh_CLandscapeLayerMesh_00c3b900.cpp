extern int g_landLayerMeshGlobal;

struct CLandscapeLayerMesh {
    int m_param;      // +0x00
    int m_global;     // +0x04
    int m_f08;        // +0x08
    int m_f0c;        // +0x0c
    short m_f10;      // +0x10
    short m_f12;      // +0x12
    int m_f14;        // +0x14
    int m_f18;        // +0x18
    int m_f1c;        // +0x1c
    CLandscapeLayerMesh(int arg);
};

CLandscapeLayerMesh::CLandscapeLayerMesh(int arg)
{
    this->m_param = arg;
    this->m_global = g_landLayerMeshGlobal;
    this->m_f08 = 0;
    this->m_f0c = 0;
    this->m_f10 = 0;
    this->m_f12 = 0;
    this->m_f14 = 0;
    this->m_f18 = 0;
    this->m_f1c = 0;
}