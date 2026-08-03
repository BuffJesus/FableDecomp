struct CPersistBlock
{
    void *pUnused;   // +0x0
    int   nCount;    // +0x4
};

class CPersistContext
{
public:
    CPersistBlock *m_pBlock; // +0x0
};

class CThingBase
{
public:
    virtual ~CThingBase() {}
    bool SerialiseGameState(CPersistContext *ctx);
};

// CThingBase::SerialiseGameState is defined in another translation unit
// in the real engine; declaration-only here so a /c compile of this file
// still emits a genuine call instruction instead of inlining a stub body.

class CThingCreatureBase : public CThingBase
{
public:
    bool SerialiseGameState(CPersistContext *ctx);

    unsigned char SerialiseElements(unsigned char flag, void *pStart, void *pEnd);

    char *m_pBuffer;        // +0x4
    int   m_pad08;          // +0x8
    int   m_pad0c;          // +0xc
    int   m_pad10;          // +0x10
    int   m_pad14;          // +0x14
    int   m_pad18;          // +0x18
    unsigned char m_bFlag;  // +0x1c
    unsigned char m_pad1d;
    unsigned char m_pad1e;
    unsigned char m_pad1f;
    int   m_nOffset;        // +0x20
};

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