#include <cstdio>
#include <cstring>

// Behaviour model of CIdleSchedulerDef::Copy:
//   1) copy base-class subobject
//   2) copy the member subobject at +0x3c

struct Obj {
    unsigned char base[0x3c];
    unsigned char member[0x20];
};

static int g_base_copied = 0;
static int g_member_copied = 0;

static void BaseCopy(Obj* self, const Obj* src) {
    memcpy(self->base, src->base, sizeof(self->base));
    g_base_copied = 1;
}
static void MemberCopy(unsigned char* self, const unsigned char* src) {
    memcpy(self, src, 0x20);
    g_member_copied = 1;
}

static void Copy(Obj* self, const Obj* src) {
    BaseCopy(self, src);
    MemberCopy((unsigned char*)self + 0x3c, (const unsigned char*)src + 0x3c);
}

int main() {
    Obj src, dst;
    for (int i = 0; i < 0x3c; ++i) src.base[i] = (unsigned char)(i + 1);
    for (int i = 0; i < 0x20; ++i) src.member[i] = (unsigned char)(0x80 + i);
    memset(&dst, 0, sizeof(dst));

    Copy(&dst, &src);

    int ok = g_base_copied && g_member_copied
        && memcmp(dst.base, src.base, 0x3c) == 0
        && memcmp(dst.member, src.member, 0x20) == 0;

    if (ok) printf("IDLESCHED_COPY_OK\n");
    else printf("FAIL\n");
    return 0;
}