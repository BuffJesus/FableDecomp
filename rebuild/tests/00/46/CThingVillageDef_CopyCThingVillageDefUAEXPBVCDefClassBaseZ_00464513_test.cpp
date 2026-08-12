#include <stdio.h>
#include <string.h>

class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* src) = 0;
};

class CThingBaseDef : public CDefClassBase {
public:
    void CopyBase(const CDefClassBase* src);
    char pad[0x70 - 4];
};

struct VillageData {
    unsigned char bytes[14];
};

class CThingVillageDef : public CThingBaseDef {
public:
    VillageData m_data;
    virtual void Copy(const CDefClassBase* src);
};

// base copy: mark that it ran, copy the pad region
static int g_baseRan = 0;
void CThingBaseDef::CopyBase(const CDefClassBase* src)
{
    g_baseRan = 1;
    const CThingBaseDef* s = static_cast<const CThingBaseDef*>(src);
    memcpy(pad, s->pad, sizeof(pad));
}

void CThingVillageDef::Copy(const CDefClassBase* src)
{
    const CThingVillageDef* s = static_cast<const CThingVillageDef*>(src);
    CThingBaseDef::CopyBase(src);
    m_data = s->m_data;
}

int main()
{
    CThingVillageDef src, dst;
    for (int i = 0; i < 14; ++i) src.m_data.bytes[i] = (unsigned char)(i + 1);
    for (int i = 0; i < (int)sizeof(src.pad); ++i) src.pad[i] = (char)(i & 0x7F);
    memset(dst.m_data.bytes, 0, 14);
    memset(dst.pad, 0, sizeof(dst.pad));
    g_baseRan = 0;

    dst.Copy(&src);

    int ok = g_baseRan;
    for (int i = 0; i < 14; ++i)
        if (dst.m_data.bytes[i] != (unsigned char)(i + 1)) ok = 0;
    for (int i = 0; i < (int)sizeof(src.pad); ++i)
        if (dst.pad[i] != (char)(i & 0x7F)) ok = 0;

    if (ok) { printf("VILLAGE_COPY_OK\n"); return 0; }
    printf("VILLAGE_COPY_FAIL\n"); return 1;
}