#include <cstdio>

struct CPersistBlock
{
    void *pUnused;
    int   nCount;
};

class CPersistContext
{
public:
    CPersistBlock *m_pBlock;
};

class CThingBase
{
public:
    virtual ~CThingBase() {}
    bool SerialiseGameState(CPersistContext *ctx);
};

static int g_baseCalls = 0;
static bool g_baseReturn = true;

bool CThingBase::SerialiseGameState(CPersistContext *ctx)
{
    (void)ctx;
    g_baseCalls++;
    return g_baseReturn;
}

class CThingCreatureBase : public CThingBase
{
public:
    bool SerialiseGameState(CPersistContext *ctx);

    unsigned char SerialiseElements(unsigned char flag, void *pStart, void *pEnd);

    char *m_pBuffer;
    int   m_pad08;
    int   m_pad0c;
    int   m_pad10;
    int   m_pad14;
    int   m_pad18;
    unsigned char m_bFlag;
    unsigned char m_pad1d;
    unsigned char m_pad1e;
    unsigned char m_pad1f;
    int   m_nOffset;
};

static int g_elemCalls = 0;
static unsigned char g_lastFlag = 0;
static void *g_lastStart = 0;
static void *g_lastEnd = 0;

unsigned char CThingCreatureBase::SerialiseElements(unsigned char flag, void *pStart, void *pEnd)
{
    g_elemCalls++;
    g_lastFlag = flag;
    g_lastStart = pStart;
    g_lastEnd = pEnd;
    return 0x55;
}

bool CThingCreatureBase::SerialiseGameState(CPersistContext *ctx)
{
    if (CThingBase::SerialiseGameState(ctx))
    {
        CPersistBlock *pBlk = ctx->m_pBlock;
        if (pBlk != 0)
        {
            unsigned int nCount = (unsigned int)pBlk->nCount;
            if (nCount > 0)
            {
                int nOffset = m_nOffset;
                char *pStart = m_pBuffer + nOffset;
                char *pEnd = pStart + nCount;

                unsigned char bResult = SerialiseElements(m_bFlag, pStart, pEnd);

                m_nOffset = nOffset + nCount;
                m_bFlag = bResult;
            }
        }
        return true;
    }
    return false;
}

int main()
{
    char buffer[256];
    CThingCreatureBase obj;
    obj.m_pBuffer = buffer;
    obj.m_pad08 = 0; obj.m_pad0c = 0; obj.m_pad10 = 0; obj.m_pad14 = 0; obj.m_pad18 = 0;
    obj.m_bFlag = 7;
    obj.m_nOffset = 10;

    CPersistBlock blk;
    blk.pUnused = 0;
    blk.nCount = 20;

    CPersistContext ctx;
    ctx.m_pBlock = &blk;

    bool ok = obj.SerialiseGameState(&ctx);

    bool pass = ok
        && g_baseCalls == 1
        && g_elemCalls == 1
        && g_lastFlag == 7
        && g_lastStart == (void *)(buffer + 10)
        && g_lastEnd == (void *)(buffer + 30)
        && obj.m_nOffset == 30
        && obj.m_bFlag == 0x55;

    // second scenario: null block -> early true, no element call
    g_elemCalls = 0; g_baseCalls = 0;
    CPersistContext ctx2;
    ctx2.m_pBlock = 0;
    bool ok2 = obj.SerialiseGameState(&ctx2);
    pass = pass && ok2 && g_elemCalls == 0 && g_baseCalls == 1;

    // third scenario: zero count -> early true, no element call
    CPersistBlock blk3; blk3.pUnused = 0; blk3.nCount = 0;
    CPersistContext ctx3; ctx3.m_pBlock = &blk3;
    g_elemCalls = 0;
    bool ok3 = obj.SerialiseGameState(&ctx3);
    pass = pass && ok3 && g_elemCalls == 0;

    // fourth scenario: base call fails -> overall false, no element call
    g_baseReturn = false;
    g_elemCalls = 0; g_baseCalls = 0;
    CPersistBlock blk4; blk4.pUnused = 0; blk4.nCount = 5;
    CPersistContext ctx4; ctx4.m_pBlock = &blk4;
    bool ok4 = obj.SerialiseGameState(&ctx4);
    pass = pass && !ok4 && g_baseCalls == 1 && g_elemCalls == 0;
    g_baseReturn = true;

    if (pass)
        std::printf("SERIALISE_GAMESTATE_OK\n");
    else
        std::printf("SERIALISE_GAMESTATE_FAIL\n");

    return 0;
}