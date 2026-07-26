#include <cstdio>

int g_landLayerMeshGlobal = 0x12345678;

struct CLandscapeLayerMesh {
    int m_param;
    int m_global;
    int m_f08;
    int m_f0c;
    short m_f10;
    short m_f12;
    int m_f14;
    int m_f18;
    int m_f1c;
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

int main()
{
    CLandscapeLayerMesh obj(0x99);
    if (obj.m_param != 0x99) { std::printf("FAIL: param\n"); return 1; }
    if (obj.m_global != 0x12345678) { std::printf("FAIL: global\n"); return 1; }
    if (obj.m_f08 || obj.m_f0c || obj.m_f10 || obj.m_f12 || obj.m_f14 || obj.m_f18 || obj.m_f1c) { std::printf("FAIL: zero\n"); return 1; }
    std::printf("CLandscapeLayerMesh_00c3b900_TEST PASS\n");
    return 0;
}