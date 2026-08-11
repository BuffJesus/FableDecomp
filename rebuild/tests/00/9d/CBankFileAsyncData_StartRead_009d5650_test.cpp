#include <stdio.h>

struct Arg2c { int a, b, c, d; };

struct Reader {
    int Read(Arg2c* p);
};

struct Inner {
    char pad[0x110];
    Reader* m110;
};

struct CBankFileAsyncData {
    char    pad0[0x18];
    int     f18;
    char    pad1c[0x28 - 0x1c];
    Inner*  f28;
    Arg2c   f2c;

    void StartRead();
};

static Arg2c* g_seen = 0;
int Reader::Read(Arg2c* p) {
    g_seen = p;
    return p->a + p->b + p->c + p->d + 1000;
}

void CBankFileAsyncData::StartRead()
{
    Reader* rd = this->f28->m110;
    this->f18 = rd->Read(&this->f2c);
}

int main() {
    Reader reader;
    Inner innr;
    innr.m110 = &reader;

    CBankFileAsyncData obj;
    obj.f18 = -1;
    obj.f28 = &innr;
    obj.f2c.a = 1; obj.f2c.b = 2; obj.f2c.c = 3; obj.f2c.d = 4;

    Arg2c* expect = &obj.f2c;
    obj.StartRead();

    bool ok = (obj.f18 == 1010) && (g_seen == expect);
    printf("f18=%d seen_matches=%d\n", obj.f18, (int)(g_seen == expect));
    if (ok) printf("STARTREAD_OK\n");
    else    printf("STARTREAD_FAIL\n");
    return ok ? 0 : 1;
}