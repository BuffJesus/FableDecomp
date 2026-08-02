#include <cstdio>
#include <cstring>

#pragma pack(push, 1)
struct CCharStringData
{
    char* text;
    unsigned char pad[9];
    unsigned int refCount;

    void AppendData(const char* src, int len);
};
#pragma pack(pop)

char g_emptyCharStringBuf = 0;

struct CCharString
{
    CCharStringData* data;

    void MakeOwnCopyOfData();
    CCharString* operator+=(CCharString* rhs);
};

static int g_makeOwnCopyCalls = 0;
static int g_appendCalls = 0;
static char g_lastAppendSrc[256];
static int g_lastAppendLen = 0;

void CCharString::MakeOwnCopyOfData()
{
    ++g_makeOwnCopyCalls;
}

void CCharStringData::AppendData(const char* src, int len)
{
    ++g_appendCalls;
    std::memcpy(g_lastAppendSrc, src, (size_t)len);
    g_lastAppendSrc[len] = 0;
    g_lastAppendLen = len;
}

extern "C" CCharString* __fastcall CCharString_operator_plusequals(CCharString* self, void*, CCharString* rhs)
{
    if (self->data != 0)
    {
        self->MakeOwnCopyOfData();

        CCharStringData* rhsData = rhs->data;
        char* rhsText = &g_emptyCharStringBuf;
        if (rhsData != 0)
        {
            rhsText = rhsData->text;
        }

        CCharStringData* selfData = self->data;

        int len = 0;
        if (rhsText[0] != 0)
        {
            do
            {
                ++len;
            } while (rhsText[len] != 0);
        }

        selfData->AppendData(rhsText, len);

        return self;
    }

    if (rhs != self)
    {
        CCharStringData* rhsData = rhs->data;
        if (rhsData != 0)
        {
            self->data = rhsData;
            ++rhsData->refCount;
        }
    }

    return self;
}

int main()
{
    /* Case 1: self->data == 0, rhs has data -> share-assign, refcount++ */
    {
        CCharStringData rhsData;
        char rhsText[] = "hello";
        rhsData.text = rhsText;
        rhsData.refCount = 8;

        CCharString self; self.data = 0;
        CCharString rhs; rhs.data = &rhsData;

        CCharString* ret = CCharString_operator_plusequals(&self, 0, &rhs);

        if (ret != &self) { std::printf("FAIL ret1\n"); return 1; }
        if (self.data != &rhsData) { std::printf("FAIL share1\n"); return 1; }
        if (rhsData.refCount != 9) { std::printf("FAIL refcount1 %u\n", rhsData.refCount); return 1; }
    }

    /* Case 2: self->data == 0, rhs->data == 0 -> stays null, no crash */
    {
        CCharString self; self.data = 0;
        CCharString rhs; rhs.data = 0;

        CCharString* ret = CCharString_operator_plusequals(&self, 0, &rhs);

        if (ret != &self) { std::printf("FAIL ret2\n"); return 1; }
        if (self.data != 0) { std::printf("FAIL share2\n"); return 1; }
    }

    /* Case 3: self->data == 0, rhs == self (self-append of empty) -> no-op, no refcount touch */
    {
        CCharString self; self.data = 0;

        CCharString* ret = CCharString_operator_plusequals(&self, 0, &self);

        if (ret != &self) { std::printf("FAIL ret3\n"); return 1; }
        if (self.data != 0) { std::printf("FAIL share3\n"); return 1; }
    }

    /* Case 4: self->data != 0 -> MakeOwnCopy + concat path, rhs has data */
    {
        g_makeOwnCopyCalls = 0;
        g_appendCalls = 0;
        g_lastAppendLen = -1;

        CCharStringData selfData;
        char selfText[] = "world";
        selfData.text = selfText;

        CCharStringData rhsData;
        char rhsText[] = "The quick brown fox";
        rhsData.text = rhsText;

        CCharString self; self.data = &selfData;
        CCharString rhs; rhs.data = &rhsData;

        CCharString* ret = CCharString_operator_plusequals(&self, 0, &rhs);

        if (ret != &self) { std::printf("FAIL ret4\n"); return 1; }
        if (g_makeOwnCopyCalls != 1) { std::printf("FAIL makeowncopy4 %d\n", g_makeOwnCopyCalls); return 1; }
        if (g_appendCalls != 1) { std::printf("FAIL appendcalls4 %d\n", g_appendCalls); return 1; }
        if (g_lastAppendLen != 19) { std::printf("FAIL appendlen4 %d\n", g_lastAppendLen); return 1; }
        if (std::strcmp(g_lastAppendSrc, "The quick brown fox") != 0) { std::printf("FAIL appendsrc4 %s\n", g_lastAppendSrc); return 1; }
    }

    /* Case 5: self->data != 0, rhs->data == 0 -> concat with empty buffer, len 0 */
    {
        g_makeOwnCopyCalls = 0;
        g_appendCalls = 0;
        g_lastAppendLen = -1;

        CCharStringData selfData;
        char selfText[] = "world";
        selfData.text = selfText;

        CCharString self; self.data = &selfData;
        CCharString rhs; rhs.data = 0;

        CCharString* ret = CCharString_operator_plusequals(&self, 0, &rhs);

        if (ret != &self) { std::printf("FAIL ret5\n"); return 1; }
        if (g_makeOwnCopyCalls != 1) { std::printf("FAIL makeowncopy5 %d\n", g_makeOwnCopyCalls); return 1; }
        if (g_appendCalls != 1) { std::printf("FAIL appendcalls5 %d\n", g_appendCalls); return 1; }
        if (g_lastAppendLen != 0) { std::printf("FAIL appendlen5 %d\n", g_lastAppendLen); return 1; }
    }

    std::printf("CCHARSTRING_OPERATOR_PLUSEQUALS_0099F0A0 PASS\n");
    return 0;
}