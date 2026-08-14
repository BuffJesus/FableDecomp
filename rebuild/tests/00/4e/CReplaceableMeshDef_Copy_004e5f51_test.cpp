#include <cstdio>

struct CDefClassBase;

struct Member28 { int v; };

struct CReplaceableMeshDef {
    char basePad[0x28];
    Member28 member;
};

static int g_baseCopied = 0;
static int g_memberSrcVal = -1;

void __fastcall BaseCopy(void* self, void* /*edx*/, CDefClassBase const* src)
{
    g_baseCopied = 1;
    (void)self; (void)src;
}
void __fastcall MemberCopy(Member28* self, void* /*edx*/, Member28 const* src)
{
    self->v = src->v;
    g_memberSrcVal = src->v;
}

void __fastcall CReplaceableMeshDef_Copy(CReplaceableMeshDef* self, void* /*edx*/, CDefClassBase const* src)
{
    BaseCopy(self, 0, src);
    MemberCopy(&self->member, 0, (Member28 const*)((char const*)src + 0x28));
}

int main()
{
    // build a "source" blob: bytes 0..0x27 base, then member.v at +0x28
    char srcBlob[0x2c];
    for (int i = 0; i < 0x2c; ++i) srcBlob[i] = (char)i;
    *(int*)(srcBlob + 0x28) = 0x1234abcd;

    CReplaceableMeshDef dst;
    for (int i = 0; i < (int)sizeof(dst); ++i) ((char*)&dst)[i] = 0;

    CReplaceableMeshDef_Copy(&dst, 0, (CDefClassBase const*)srcBlob);

    if (g_baseCopied == 1 && dst.member.v == 0x1234abcd && g_memberSrcVal == 0x1234abcd)
        printf("PARITY_OK\n");
    else
        printf("FAIL base=%d mem=%08x srcval=%08x\n", g_baseCopied, dst.member.v, g_memberSrcVal);
    return 0;
}