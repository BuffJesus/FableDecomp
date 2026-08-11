#include <cstdio>

struct CDefClassBase;

struct CBuyHouseBase {
    char pad[0xd];
    char field_d;   // +0xd
    void BaseCopy(CDefClassBase* src);
};

struct CBuyHouseDef : CBuyHouseBase {
    void Copy(CDefClassBase* src);
};

static int g_called = 0;
static CBuyHouseBase* g_self = 0;
static CDefClassBase* g_src = 0;

void CBuyHouseBase::BaseCopy(CDefClassBase* src)
{
    g_called = 1;
    g_self = this;
    g_src = src;
}

void CBuyHouseDef::Copy(CDefClassBase* src)
{
    this->CBuyHouseBase::BaseCopy(src);
    this->field_d = ((CBuyHouseBase*)src)->field_d;
}

int main()
{
    CBuyHouseDef dst;
    CBuyHouseDef src;
    for (int i = 0; i < (int)sizeof(dst); ++i) ((char*)&dst)[i] = 0;
    for (int i = 0; i < (int)sizeof(src); ++i) ((char*)&src)[i] = 0;
    src.field_d = (char)0x5A;
    dst.field_d = (char)0x11;

    dst.Copy((CDefClassBase*)(CBuyHouseBase*)&src);

    if (g_called != 1) { printf("FAIL called\n"); return 1; }
    if (g_self != (CBuyHouseBase*)&dst) { printf("FAIL self\n"); return 1; }
    if (dst.field_d != (char)0x5A) { printf("FAIL field\n"); return 1; }

    printf("COPY_OK_5A\n");
    return 0;
}