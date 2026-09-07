class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();

private:
    void* storage_;
};

struct LipSyncEntry;

class LipSyncEntryArray
{
public:
    LipSyncEntry* begin;
    LipSyncEntry* end;
    void EraseRange(LipSyncEntry* first, LipSyncEntry* last);
};

class GossipMap
{
public:
    LipSyncEntryArray& operator[](const CCharString& key);
};

extern GossipMap DAT_013bae44;

class CGameScriptInterface
{
public:
    virtual void ClearGossip(CCharString gossipName);
};

void CGameScriptInterface::ClearGossip(CCharString gossipName)
{
    CCharString localKey(gossipName);
    LipSyncEntryArray& entries = DAT_013bae44[localKey];
    entries.EraseRange(entries.begin, entries.end);
}