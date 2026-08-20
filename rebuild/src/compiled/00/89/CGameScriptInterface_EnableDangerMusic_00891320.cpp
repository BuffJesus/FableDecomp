enum EMusicSetType { EMS_A };

struct CMusicTarget
{
    void CacheMusicSet(EMusicSetType t);
};

struct CInner
{
    char pad_0000[0x6C];
    CMusicTarget* target_006C;
};

struct CGameScriptInterface
{
    char pad_0000[0x04];
    CInner* inner_0004;
    void CacheMusicSet(EMusicSetType t) const;
};

void CGameScriptInterface::CacheMusicSet(EMusicSetType t) const
{
    inner_0004->target_006C->CacheMusicSet(t);
}